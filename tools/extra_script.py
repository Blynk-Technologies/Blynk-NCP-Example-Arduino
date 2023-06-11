import time

Import("env")

class dotdict(dict):
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

pioenv = env["PIOENV"]

custom_ncp = dotdict({})
custom_ncp.firmware     = env.GetProjectOption("custom_ncp.firmware", None)
custom_ncp.upload_speed = env.GetProjectOption("custom_ncp.upload_speed", "460800")
custom_ncp.manual_reset = env.GetProjectOption("custom_ncp.manual_reset", False)
custom_ncp.erase_all    = env.GetProjectOption("custom_ncp.erase_all", False)
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

def check_exec(cmd):
    if env.Execute(cmd):
        env.Exit(1)

def upload_ncp(*args, **kwargs):

    if custom_ncp.firmware is None:
        raise Exception("custom_ncp.firmware not specified")

    # Build and upload the flasher utility
    check_exec(f"pio run -d tools/BlynkNcpFlasher -e {pioenv} --target upload")

    time.sleep(3)
    if custom_ncp.manual_reset:
        input(hint_bootloader)

    check_exec(' '.join(["pio", "pkg", "exec",
        "-p", "tool-esptoolpy", "--", "esptool.py",
          "--baud",   custom_ncp.upload_speed,
          "--before", custom_ncp.before_upload,
          "--after",  custom_ncp.after_upload,
        "write_flash",
          #"--flash_mode", "dio", TODO: flash_mode
          #"--flash_freq", "40m", TODO: f_flash
          "--flash_size", "detect",
          "--erase-all" if custom_ncp.erase_all else "",
        "0", custom_ncp.firmware
    ]))

    if custom_ncp.manual_reset:
        input(hint_reset)

env.AddCustomTarget(
    name="upload_ncp",
    dependencies=None,
    actions=upload_ncp,
    title="Upload Blynk NCP",
    description="Replace NINA firmware with Blynk.Edgent NCP"
)
