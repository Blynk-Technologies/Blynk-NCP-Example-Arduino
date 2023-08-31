import os
import sys
import requests
import contextlib
import platform
import struct
from pathlib import Path
from zipfile import ZipFile, ZipInfo

_local_path = os.path.realpath(os.path.dirname(__file__))
_ambd_flash_tool = Path(_local_path, "ambd_flash_tool")

class ZipFileWithPermissions(ZipFile):
    """ Custom ZipFile class handling file permissions. """
    def _extract_member(self, member, targetpath, pwd):
        if not isinstance(member, ZipInfo):
            member = self.getinfo(member)

        targetpath = super()._extract_member(member, targetpath, pwd)

        attr = member.external_attr >> 16
        if attr != 0:
            os.chmod(targetpath, attr)
        return targetpath


@contextlib.contextmanager
def pushd(new_dir):
    previous_dir = os.getcwd()
    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(previous_dir)

try:
    import serial
except ImportError:
    print("Installing pyserial module")
    res = os.system("pip3 install pyserial")
    if res != 0:
        print("pyserial module installation failed")
        sys.exit(1)
    import serial

if os.name == 'nt':  # sys.platform == 'win32':
    from serial.tools.list_ports_windows import comports
elif os.name == 'posix':
    from serial.tools.list_ports_posix import comports
else:
    raise ImportError("Sorry: no implementation for your platform ('{}') available".format(os.name))

# List of supported board USB IDs.  Each board is a tuple of unique USB vendor
# ID, USB product ID.
BOARD_IDS = \
    [{
        "name": "wio terminal",
        "info": ("2886", "802D"),
        "isbootloader": False
    },
    {
        "name": "wio terminal",
        "info": ("2886", "002D"),
        "isbootloader": True
    }]

def getAllPortInfo():
    return comports(include_links=False)

def getAvailableBoard():
    for info in getAllPortInfo():
        port, desc, hwid = info
        ii = hwid.find("VID:PID")
        #hwid: USB VID:PID=2886:002D SER=4D68990C5337433838202020FF123244 LOCATION=7-3.1.3:1.
        if ii != -1:
            for b in  BOARD_IDS:
                (vid, pid) = b["info"]
                if vid == hwid[ii + 8: ii + 8 + 4] and pid == hwid[ii + 8 + 5 :ii + 8 + 5 + 4 ]:
                    if b["isbootloader"] == True :
                        return port, True
                    else:
                        return port, False
    return None, False

def stty(port, baud):
    if os.name == "posix":
        if platform.uname().system == "Darwin":
            return os.system(f"stty -f {port} {baud}")
        else:
            return os.system(f"stty -F {port} {baud}")
    elif os.name == "nt":
        return os.system(f"MODE {port}:BAUD={baud} PARITY=N DATA=8")

    raise Exception("stty not supported")

def downloadFile(url, filename):
    with requests.get(url, stream=True) as r:
        r.raise_for_status()
        with open(filename, 'wb') as f:
            for chunk in r.iter_content(chunk_size=8192):
                f.write(chunk)

def getFlashTool():
    if not _ambd_flash_tool.exists():
        zipfn = str(_ambd_flash_tool) + ".zip"
        downloadFile("https://github.com/Seeed-Studio/ambd_flash_tool/archive/refs/heads/master.zip", zipfn)
        with ZipFileWithPermissions(zipfn) as zf:
            zf.extractall(_local_path)
        Path(_local_path, "ambd_flash_tool-master").rename(_ambd_flash_tool)
        Path(zipfn).unlink()

    tool = Path(_ambd_flash_tool, 'tool')
    _platform = platform.platform()
    if 'Windows' in _platform:
        tool = str(Path(tool, 'windows', "amebad_image_tool.exe"))
    elif 'Linux' in _platform:
        tool = str(Path(tool, 'linux', 'amebad_image_tool'))
    elif _platform.find('Darwin') >= 0 or _platform.find('macOS') >= 0:
        tool = str(Path(tool, 'macos', 'amebad_image_tool'))
    else:
        raise Exception("Platform not supported")
    return tool

def makeEmptyImage(length):
    empty = struct.pack('B', 0xFF) * 1024
    with open(Path(_ambd_flash_tool, "km0_boot_all.bin"), "wb") as f:
        for i in range(8):
            f.write(empty)

    with open(Path(_ambd_flash_tool, "km4_boot_all.bin"), "wb") as f:
        for i in range(4):
            f.write(empty)

    with open(Path(_ambd_flash_tool, "km0_km4_image2.bin"), "wb") as f:
        for i in range(length - 12):
            f.write(empty)

def extractFirmware(filename):
    with ZipFile(filename) as zf:
        zf.extractall(_ambd_flash_tool)

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='WIO Terminal flasher utility')
    parser.add_argument('--erase-all', action="store_true", help='Erase all flash')
    parser.add_argument('firmware', metavar='ZIP_FILE',  help='firmware zip file')
    args = parser.parse_args()

    firmware = os.path.realpath(args.firmware)

    tool = getFlashTool()
    port, isbootloader = getAvailableBoard()
    if port == None:
        print("The device is not plugged in")
        sys.exit(1)

    with pushd(_ambd_flash_tool):
        if args.erase_all:
            print("Erasing, please wait...")
            makeEmptyImage(2048)
            os.system(f"{tool} {port}")
        print("Flashing, please wait...")
        extractFirmware(firmware)
        os.system(f"{tool} {port}")

    print("Done. Rebooting NCP")
    stty(port, 2400)

