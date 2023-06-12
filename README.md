# Blynk.NCP

This is an example of Blynk used in a [dual-MCU configuration](https://docs.google.com/presentation/d/1aP2sQWB0J9EWj8Y1h5qeyfm2aFwaNSUKnCE-k7zxVnk/present):

- Application/business logic resides on the **Primary MCU**
- Connectivity is completely off-loaded to the **Network Co-Processor**

## Features

- **Blynk.Inject**: connect your devices easily using **Blynk.App** (iOS/Android) or **Blynk.Console** (Web Dashboard)
  - `BLE`-assisted device provisioning for the best end-user experience
  - `WiFiAP`-based provisioning for devices without BLE support
- Secure **Blynk.Cloud** connection that provides simple API for:
  - Virtual Pins, Events, Metadata access
  - Time, Timezone and Location
  - Persistent automations (works even if device is offline)
- Network Manager:
  - WiFi (up to 16 saved networks), Ethernet, Cellular
- **Blynk.Air** - automatic Over The Air firmware updates using **Blynk.Cloud**
  - Both NCP and MCU update
  - Local upgrade using **Blynk.App** (during provisioning)

Additional services that can be provided by the Blynk.NCP:

- State indication - requires a monochrome/RGB/addressable LED attached to the NCP
- User button - requires a momentary push button attached to the NCP
- Non-volatile storage for [Preferences](https://github.com/vshymanskyy/Preferences)
- File System storage
- Factory testing and provisioning

## Getting started

This is a **PlatformIO** project. We recommend using [**VSCode**][pio_vscode] or [**PIO CLI**][pio_cli].  

Flash the Blynk.NCP firmware:

```sh
pio run -e rp2040connect -t upload_ncp
```

> **Note:** this overwrites both the MCU and the NINA module firmware.  
You can [restore the stock NCP firmware][restore] easily.

Open `src/main.cpp` and fill in [information from your Blynk Template](https://bit.ly/BlynkInject):

```cpp
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "MyDevice"
```

Build and flash the example project, run the serial monitor:

```sh
pio run -e rp2040connect -t upload -t monitor
```

Use your Blynk.App (iOS/Android) to connect the device to the cloud.


## Supported boards

Blynk.NCP is directly compatible with a set of Dual-MCU boards:

Board                            |                 | MCU      | NCP         | Connectivity | Provisioning
:--                              | ---             | :---     | :---        | ---          | ---
[Arduino Nano RP2040 Connect][1] | `rp2040connect` | `RP2040` | `NINA-W102` | WiFi 2.4     | BLE
[Arduino Nano 33 IoT][2]         | `nano33iot`     | `SAMD21` | `NINA-W102` | WiFi 2.4     | BLE
[Arduino MKR WiFi 1010][3]       | `mkrwifi1010`   | `SAMD21` | `NINA-W102` | WiFi 2.4     | BLE
[Arduino UNO R4 WiFi][4]         | WIP             | `RA4M1`  | `ESP32s3`   | WiFi 2.4     | BLE
[Arduino Portenta C33][5]        | WIP             | `RA4M1`  | `ESP32c3`   | WiFi 2.4     | BLE
[TTGO T-PicoC3][6]               | WIP             | `RP2040` | `ESP32c3`   | WiFi 2.4     | BLE
[RPi Pico][7] + [ESP8266][8]     | `pico_esp8266`  | `RP2040` | `ESP8266`   | WiFi 2.4     | WiFiAP

## Custom boards

1. Flash your connectivity module with the [**Blynk.NCP** firmware](https://github.com/blynkkk/BlynkNcpDriver/releases/latest)
2. Attach NCP to the MCU of your choice (i.e. `STM32`, `ATmega`, `RP2040` or `ATSAMD`)
3. Use the Blynk-provided library to communicate with the NCP:
   - [`BlynkNcpDriver`](https://github.com/blynkkk/BlynkNcpDriver) - a low-level, `C99` compatible driver with minimal dependencies
   - [`Blynk library`](https://github.com/blynkkk/blynk-library) provides an optional `C++11` convenience wrapper for the driver

Connectivity module              | NCP firmware         | Chipset         | Connectivity | Provisioning | Interaction
:--                              | :---                 | :---            | ---          | ---          | ---
[Adafruit AirLift][20]           | `generic_esp32_4M`   | `ESP32`         | WiFi 2.4     | BLE          | RGB LED
[Macchina SuperB][21]            | `generic_esp32_4M`   | `ESP32`         | WiFi 2.4     | BLE          | Blue LED
ESP-WROOM-32 4MB<br>(no PSRAM)   | `generic_esp32_4M`   | `ESP32`         | WiFi 2.4     | BLE          |
[Seeed WT32-ETH01][22]           | `wt32_eth01`         | `ESP32+LAN8720` | WiFi 2.4, Ethernet | BLE    |
[Witty Cloud][23]                | `generic_esp8266_4M` | `ESP8266`       | WiFi 2.4     | WiFiAP       | RGB LED, User Button
[DFRobot WiFi Bee][24]           | `generic_esp8266_4M` | `ESP8266`       | WiFi 2.4     | WiFiAP       | User Button
ESP-07S, ESP-12F                 | `generic_esp8266_4M` | `ESP8266`       | WiFi 2.4     | WiFiAP       |


## Disclaimer

> The community edition of **Blynk.NCP** is available for personal usage and evaluation.  
If you're interested in using **Blynk.NCP** for commercial applications, feel free to [contact Blynk][blynk_sales]. Thank you!


[blynk_sales]: https://blynk.io/en/contact-us-business
[pio_vscode]: https://docs.platformio.org/en/stable/integration/ide/vscode.html#ide-vscode
[pio_cli]: https://docs.platformio.org/en/stable/core/index.html
[restore]: ./docs/RestoreFirmware.md

[1]: https://store.arduino.cc/products/arduino-nano-rp2040-connect
[2]: https://store.arduino.cc/products/arduino-nano-33-iot
[3]: https://store.arduino.cc/products/arduino-mkr-wifi-1010
[4]: https://store-usa.arduino.cc/pages/unor4
[5]: https://www.arduino.cc/pro/hardware-product-portenta-c33
[6]: https://www.lilygo.cc/products/lilygo%C2%AE-t-picoc3-esp32-c3-rp2040-1-14-inch-lcd-st7789v
[7]: https://www.raspberrypi.com/products/raspberry-pi-pico
[8]: https://www.waveshare.com/pico-esp8266.htm

[20]: https://www.adafruit.com/product/4201
[21]: https://www.macchina.cc/catalog/m2-accessories/superb
[22]: https://www.seeedstudio.com/Ethernet-module-based-on-ESP32-series-WT32-ETH01-p-4736.html
[23]: https://protosupplies.com/product/esp8266-witty-cloud-esp-12f-wifi-module/
[24]: https://www.dfrobot.com/product-1279.html
