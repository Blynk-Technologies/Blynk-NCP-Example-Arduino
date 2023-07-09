import os
import time
import requests
import fnmatch
import json
import subprocess

Import("env")

class dotdict(dict):
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

pioenv = env["PIOENV"]

custom_ncp = dotdict({})
custom_ncp.flasher      = env.GetProjectOption("custom_ncp.flasher", "BlynkNcpFlasher")
custom_ncp.firmware     = env.GetProjectOption("custom_ncp.firmware", None)
custom_ncp.upload_speed = env.GetProjectOption("custom_ncp.upload_speed", "460800")
custom_ncp.manual_reset = env.GetProjectOption("custom_ncp.manual_reset", False)
custom_ncp.erase_all    = env.GetProjectOption("custom_ncp.erase_all", True)
custom_ncp.use_stub     = env.GetProjectOption("custom_ncp.use_stub", True)
if custom_ncp.manual_reset:
    custom_ncp.before_upload = env.GetProjectOption("custom_ncp.before_upload",  "no_reset")
    custom_ncp.after_upload  = env.GetProjectOption("custom_ncp.after_upload",   "no_reset")
else:
    custom_ncp.before_upload = env.GetProjectOption("custom_ncp.before_upload",  "default_reset")
    custom_ncp.after_upload  = env.GetProjectOption("custom_ncp.after_upload",   "hard_reset")

hint_bootloader = """

Switch your ESP8266 into Firware update mode:
  1. Press and hold the Boot button (on some boards it can be marked as Flash or IO0)
  2. Press Reset(RST/EN) button shortly
  3. Release the Boot button

Press [Enter] when ready.
"""

hint_reset = """
Upload complete. Please power-cycle your device (remove the power completely).

Press [Enter] when ready.
"""

hint_no_flasher = """
Make sure that flasher utility is uploaded to the board.
Press [Enter] when ready.
"""

def check_exec(cmd):
    if env.Execute(cmd):
        env.Exit(1)

def download_file(url, filename):
    with requests.get(url, stream=True) as r:
        r.raise_for_status()
        with open(filename, 'wb') as f:
            for chunk in r.iter_content(chunk_size=8192):
                f.write(chunk)


def get_release_info(release):
    repo = "blynkkk/BlynkNcpDriver"
    now = int(time.time())

    if release is None:
        release = "latest"

    cached_info = f".pio/BlynkNCP/.cache/{release}.json"

    try:
        with open(cached_info, "r") as f:
            data = json.load(f)

        if release == "latest" and now - data["timestamp"] > 24*60*60:
            need_get_info = True
        else:
            need_get_info = False
    except:
        need_get_info = True

    if need_get_info:
        print("Getting Blynk.NCP release info")
        if release == "latest":
            url = f"https://api.github.com/repos/{repo}/releases/latest"
        else:
            url = f"https://api.github.com/repos/{repo}/releases/tags/{release}"

        with requests.get(url) as r:
            r.raise_for_status()
            data = r.json()
            data["timestamp"] = now

        os.makedirs(".pio/BlynkNCP/.cache/", exist_ok=True)
        with open(cached_info, "w") as f:
            json.dump(data, f)

    return data

def get_download_url(filename, release_info):
    for asset in release_info["assets"]:
        asset_name = asset["name"]
        if fnmatch.fnmatch(asset_name, filename):
            return (asset_name, asset["browser_download_url"])

    tag = release_info["tag_name"]
    raise Exception(f"{filename} not found in Blynk.NCP {tag}")

def fetch_ncp(filename, release = None):
    release_info = get_release_info(release)
    tag = release_info["tag_name"]
    ncp_path = f".pio/BlynkNCP/{tag}/"
    ncp_full = ncp_path + filename
    if os.path.exists(ncp_full):
        return ncp_full

    (fn, url) = get_download_url(filename, release_info)
    ncp_full = ncp_path + fn
    if not os.path.exists(ncp_full):
        print(f"Downloading {fn} ...")
        os.makedirs(ncp_path, exist_ok=True)
        download_file(url, ncp_full)
    return ncp_full

def upload_ncp(*args, **kwargs):

    if custom_ncp.firmware is None:
        raise Exception("custom_ncp.firmware not specified")

    firmware = fetch_ncp(f"BlynkNCP_{custom_ncp.firmware}.flash.bin")

    if custom_ncp.flasher == "BlynkNcpFlasher":
        # Build and upload the flasher utility
        check_exec(f"pio run -d tools/BlynkNcpFlasher -e {pioenv} --target upload")
    elif custom_ncp.flasher == "none":
        input(hint_no_flasher)
    elif custom_ncp.flasher == "direct":
        pass
    else:
        raise Exception("custom_ncp.flasher is invalid")

    time.sleep(3)
    if custom_ncp.manual_reset:
        input(hint_bootloader)

    check_exec(' '.join(["pio", "pkg", "exec",
        "-p", "tool-esptoolpy", "--", "esptool.py",
          "" if custom_ncp.use_stub else "--no-stub",
          "--baud",   custom_ncp.upload_speed,
          "--before", custom_ncp.before_upload,
          "--after",  custom_ncp.after_upload,
        "write_flash",
          #"--flash_mode", "dio", TODO: flash_mode
          #"--flash_freq", "40m", TODO: f_flash
          "--flash_size", "detect",
          "--erase-all" if custom_ncp.erase_all else "",
        "0x0", firmware
    ]))

    if custom_ncp.manual_reset:
        input(hint_reset)

env.AddCustomTarget(
    name="upload_ncp",
    dependencies=None,
    actions=upload_ncp,
    title="Upload Blynk.NCP firmware"
)
