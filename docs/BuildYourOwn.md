# Building a new device using Blynk.NCP

![Blynk.NCP connection](BlynkNCP.png)

## Supported connectivity modules

Connectivity module              | NCP firmware         | Chipset         | Connectivity | Provisioning | Interaction
:--                              | :---                 | :---            | ---          | ---          | ---
[Adafruit AirLift][20]           | `generic_esp32_4M`   | `ESP32`         | WiFi 2.4     | BLE          | RGB LED
[Macchina SuperB][21]            | `generic_esp32_4M`   | `ESP32`         | WiFi 2.4     | BLE          | Green LED
ESP-WROOM-32 4MB<br>(no PSRAM)   | `generic_esp32_4M`   | `ESP32`         | WiFi 2.4     | BLE          |
[TTGO T-Internet-POE][22]        | `lilygo_poe`         | `ESP32+LAN8720` | WiFi 2.4, Ethernet POE | BLE    |
[Seeed WT32-ETH01][23]           | `wt32_eth01`         | `ESP32+LAN8720` | WiFi 2.4, Ethernet     | BLE    |
[Witty Cloud][24]                | `generic_esp8266_4M` | `ESP8266`       | WiFi 2.4     | WiFiAP       | RGB LED, User Button
[DFRobot WiFi Bee][25]           | `generic_esp8266_4M` | `ESP8266`       | WiFi 2.4     | WiFiAP       | User Button
ESP-07S, ESP-12F                 | `generic_esp8266_4M` | `ESP8266`       | WiFi 2.4     | WiFiAP       |


## How to use

1. Flash your connectivity module with the [**Blynk.NCP** firmware](https://github.com/blynkkk/BlynkNcpDriver/releases/latest)
   - Use `BlynkNCP_generic_esp32_4M_wifiap.flash.bin`, flash at address `0`
   - This is a combined firmware, so you only need to flash a single file
2. Connect NCP module to the MCU of your choice (i.e. `STM32`, `ATmega`, `RP2040` or `ATSAMD`) using UART
3. Use the Blynk-provided library to communicate with the NCP:
   - [`BlynkNcpDriver`](https://github.com/blynkkk/BlynkNcpDriver) - a low-level, `C99` compatible driver with minimal dependencies
   - [`Blynk library`](https://github.com/blynkkk/blynk-library) provides an optional `C++11` convenience wrapper for the driver
4. Upload the firmware to your **Primary MCU**. The expected debug output looks like this:
    ```
    [1345] NCP responding (baud 115200, 2289 us)
    [1349] Blynk.NCP firmware: 0.4.6
    [1684] State: Configuration
    ```
5. Use the **Blynk mobile app** (iOS/Android) to configure your new device
