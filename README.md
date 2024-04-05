
# Blynk.NCP integration for Arduino

[![Issues](https://img.shields.io/github/issues/Blynk-Technologies/Blynk-NCP-Driver.svg)](https://github.com/Blynk-Technologies/Blynk-NCP-Driver/issues)
[![Downloads](https://img.shields.io/github/downloads/Blynk-Technologies/Blynk-NCP-Driver/total)](https://github.com/Blynk-Technologies/Blynk-NCP-Driver/releases/latest)
[![CI Status](https://img.shields.io/github/actions/workflow/status/Blynk-Technologies/Blynk-NCP-Example-Arduino/build.yml?branch=main&logo=github&label=tests)](https://github.com/Blynk-Technologies/Blynk-NCP-Example-Arduino/actions)
[![License](https://img.shields.io/github/license/Blynk-Technologies/Blynk-NCP-Example-Arduino?color=blue)](LICENSE)
[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/badges/StandWithUkraine.svg)](https://stand-with-ukraine.pp.ua)

**Blynk.NCP** is a solution that off-loads connectivity to a **Network Co-Processor (NCP)** while your application logic resides on the **Primary MCU**. This implies a [dual-Microcontroller Unit (MCU)](https://docs.google.com/presentation/d/1aP2sQWB0J9EWj8Y1h5qeyfm2aFwaNSUKnCE-k7zxVnk/present) architecture.

[**Read more about Blynk.NCP**](https://github.com/Blynk-Technologies/Blynk-NCP-Driver)

### Supported Dual-MCU Boards
This example project is compatible with a set of ready-to-use Dual-MCU boards:

Board                            |                 | 🔄 MCU / NCP   | 🌐            | ⚙️      | 🗃️
:--                              | ---             | :---           | :---         | :---    | :---
[UNO R4 WiFi][1]                 | `unoR4wifi`     | `✅ / ✅`      | WiFi 2.4     | BLE     | 4.25 MB
[Portenta C33][2]                | `portentaC33`   | `⏳ / ✅`      | WiFi 2.4     | BLE     | 512 KB
[Nano RP2040 Connect][3]         | `rp2040connect` | `✅ / ❌`      | WiFi 2.4     | BLE     | 384 KB
[Nano 33 IoT][4]                 | `nano33iot`     | `✅ / ❌`      | WiFi 2.4     | BLE     | 384 KB
[MKR WiFi 1010][5]               | `mkrwifi1010`   | `✅ / ❌`      | WiFi 2.4     | BLE     | 384 KB
[T-PicoC3][6]                    | `t_pico_c3`     | `✅ / ✅`      | WiFi 2.4     | BLE     | 512 KB
[RPi Pico][7] + [ESP8266][8]     | `pico_esp8266`  | `✅ / ✅`      | WiFi 2.4     | WiFiAP  | 1000 KB
[Wio Terminal][9]                | `⏳`            | `✅ / ❔`      | WiFi 2.4/5   | BLE     | ❔

🔄 Over-the-Air Updates, 🌐 Connectivity, ⚙️ Provisioning, 🗃️ File System

### Custom Boards
You can also [add one of the supported connectivity modules](docs/BuildYourOwn.md) to your custom board.

## Getting started

1. This is a **PlatformIO** project. Please install the [**VSCode plugin**][pio_vscode] or [**PlatformIO CLI**][pio_cli].
2. Clone [this repository][self] using `git` or download it as a [`ZIP` file][self_zip].
3. Flash the Blynk.NCP firmware (***replace `nano33iot` with your board type***):

    ```sh
    pio run -e nano33iot -t upload_ncp
    ```

> [!WARNING]  
> This overwrites both the Main MCU and the connectivity module firmware.  
> You can [restore the stock firmware][restore] easily.
4. Open `src/main.cpp` and fill in [information from your Blynk Template](https://bit.ly/BlynkInject):

    ```cpp
    #define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
    #define BLYNK_TEMPLATE_NAME         "MyDevice"
    ```
5. Upload the example project and run the serial monitor:

    ```sh
    pio run -e nano33iot -t upload -t monitor
    ```
6. Use the Blynk iOS/Android app to configure your new device.  
    Ensure that the Blynk App is installed on your smartphone.  
    Open the `Blynk App` -> click `Add New Device` -> select `Find Devices Nearby`

## Troubleshooting

If something doesn't work, try some of these solutions:

- If your board is based on a `Raspberry Pi RP2040` chip, it may fail to work on Windows. Please apply [this fix][fix_windows_rp2040]
- On `TTGO T-Pico C3`, you may need to connect `IO9` pin to `GND` to flash the NCP firmware
- Upgrade PlatformIO to the latest version. Run: `pio upgrade`
- Upgrade packages to the latest version. Run: `pio pkg update --no-save`

## Report an issue

Issues are maintained in the [Blynk NCP Driver](https://github.com/Blynk-Technologies/Blynk-NCP-Driver/issues) repository.

## Disclaimer

> The community edition of Blynk.NCP is available for personal use and evaluation.  
> If you're interested in using Blynk.NCP for commercial applications, feel free to [contact Blynk][blynk_sales]. Thank you!


[self]: https://github.com/Blynk-Technologies/Blynk-NCP-Example-Arduino
[self_zip]: https://github.com/Blynk-Technologies/Blynk-NCP-Example-Arduino/archive/refs/heads/main.zip
[blynk_sales]: https://blynk.io/en/contact-us-business
[pio_vscode]: https://docs.platformio.org/en/stable/integration/ide/vscode.html#ide-vscode
[pio_cli]: https://docs.platformio.org/en/stable/core/index.html
[restore]: ./docs/RestoreFirmware.md
[arduino_ide]: https://github.com/Blynk-Technologies/blynk-library/blob/master/examples/Blynk.Edgent/Edgent_NCP/Edgent_NCP.ino
[fix_windows_rp2040]: https://arduino-pico.readthedocs.io/en/latest/platformio.html#important-steps-for-windows-users-before-installing

[1]: https://store-usa.arduino.cc/products/uno-r4-wifi
[2]: https://store-usa.arduino.cc/products/portenta-c33
[3]: https://store-usa.arduino.cc/products/arduino-nano-rp2040-connect
[4]: https://store-usa.arduino.cc/products/arduino-nano-33-iot
[5]: https://store-usa.arduino.cc/products/arduino-mkr-wifi-1010
[6]: https://www.lilygo.cc/products/lilygo%C2%AE-t-picoc3-esp32-c3-rp2040-1-14-inch-lcd-st7789v
[7]: https://www.raspberrypi.com/products/raspberry-pi-pico
[8]: https://www.waveshare.com/pico-esp8266.htm
[9]: https://www.seeedstudio.com/Wio-Terminal-p-4509.html

