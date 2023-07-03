Import("env")

import os
import shutil

pioenv = env["PIOENV"]

os.makedirs("./build", exist_ok=True)

def copy_bin(source, target, env):
    program_path = target[0].get_path()
    shutil.copy(program_path, f"build/firmware_{pioenv}.ota.bin")

if pioenv in ["rp2040connect"]:
    # BIN from ELF
    env.AddPostAction(
        "$BUILD_DIR/${PROGNAME}.elf",
        env.VerboseAction(" ".join([
            "$OBJCOPY", "-O", "binary", "$BUILD_DIR/${PROGNAME}.elf", "$BUILD_DIR/${PROGNAME}.bin"
        ]), "Building $BUILD_DIR/${PROGNAME}.bin")
    )

if not "mbed" in pioenv:
    env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_bin)
