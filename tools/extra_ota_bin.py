Import("env")

import os
import shutil

pioenv = env["PIOENV"]
ota_bin = f"build/firmware_{pioenv}.ota.bin"

os.makedirs("./build", exist_ok=True)

def copy_bin(source, target, env):
    shutil.copy(target[0].get_path(), ota_bin)

if pioenv in ["rp2040connect"]:
    # BIN from ELF
    env.AddPostAction(
        "$BUILD_DIR/${PROGNAME}.elf",
        env.VerboseAction(" ".join([
            "$OBJCOPY", "-O", "binary", "\"$BUILD_DIR/${PROGNAME}.elf\"", ota_bin
        ]), f"Building {ota_bin}")
    )
elif not "mbed" in pioenv:
    env.AddPostAction(
        "$BUILD_DIR/${PROGNAME}.bin",
        env.VerboseAction(copy_bin, f"Building {ota_bin}")
    )

