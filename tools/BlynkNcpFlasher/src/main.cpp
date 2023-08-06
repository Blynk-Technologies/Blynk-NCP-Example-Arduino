#include <Arduino.h>

#if !defined(SerialNCP)
    #define SerialPC Serial
#endif

void ledOFF();
void ledToggle();

#ifdef SerialNCP

    // Good, use the specified SerialNCP

    void ncpInit() {}

#elif defined(ARDUINO_NANO_RP2040_CONNECT) || \
      defined(ARDUINO_SAMD_NANO_33_IOT)    || \
      defined(ARDUINO_SAMD_MKRWIFI1010)

    #define SerialNCP SerialNina

    unsigned long baud = 115200;
    int rts = -1;
    int dtr = -1;

    void ncpInit() {
      SerialNCP.begin(baud);

      pinMode(NINA_GPIO0, OUTPUT);
      pinMode(NINA_RESETN, OUTPUT);
    }

    #include "loop_nina.h"

#elif defined(ARDUINO_AVR_UNO_WIFI_REV2)

    #define SerialNCP SerialNina

    unsigned long baud = 115200;

    void ncpInit() {
      SerialNCP.begin(baud);

      pinMode(NINA_GPIO0, OUTPUT);
      pinMode(NINA_RESETN, OUTPUT);

      // Manually switch NINA module into upload mode
      digitalWrite(NINA_GPIO0, LOW);

      digitalWrite(NINA_RESETN, LOW);
      delay(100);
      digitalWrite(NINA_RESETN, HIGH);
      delay(100);
      digitalWrite(NINA_RESETN, LOW);
    }

    #include "loop_simple.h"

#elif defined(ARDUINO_SAMD_MKRVIDOR4000)

    #include <VidorPeripherals.h>

    #define SerialNCP SerialNina

    unsigned long baud = 119400;
    int rts = -1;
    int dtr = -1;

    void ncpInit() {
      FPGA.begin();
      SerialNCP.begin(baud);

      FPGA.pinMode(FPGA_NINA_GPIO0, OUTPUT);
      FPGA.pinMode(FPGA_SPIWIFI_RESET, OUTPUT);
    }

    #include "loop_vidor.h"

#elif defined(ARDUINO_UNOWIFIR4)

    #undef  SerialPC
    #define SerialPC        SerialUSB
    #define SerialNCP       Serial2

    unsigned long baud = 9600;
    int rts = -1;
    int dtr = -1;

    void ncpInit() {
      SerialNCP.begin(baud);
    }

    #include "loop_renesas.h"

#elif defined(ARDUINO_UNOR4_MINIMA)

    #define SerialNCP       Serial1

    unsigned long baud = 9600;
    int rts = -1;
    int dtr = -1;

    void ncpInit() {
      SerialNCP.begin(baud);
    }

    #include "loop_renesas.h"

#elif defined(ARDUINO_PORTENTA_C33)

    #define SerialNCP       Serial5
    #define NCP_PIN_GPIO0   (100)
    #define NCP_PIN_RESETN  (101)
    #define LED_PIN         LEDB

    unsigned long baud = 9600;
    int rts = -1;
    int dtr = -1;

    void ncpInit() {
      SerialNCP.begin(baud);

      pinMode(LEDR, OUTPUT); digitalWrite(LEDR, HIGH);
      pinMode(LEDG, OUTPUT); digitalWrite(LEDG, HIGH);

      pinMode(NCP_PIN_GPIO0, OUTPUT);
      pinMode(NCP_PIN_RESETN, OUTPUT);

      digitalWrite(NCP_PIN_GPIO0, HIGH);
      delay(100);
      digitalWrite(NCP_PIN_RESETN, HIGH);
      digitalWrite(NCP_PIN_RESETN, LOW);
      digitalWrite(NCP_PIN_RESETN, HIGH);
    }

    #include "loop_renesas.h"

#elif defined(ARDUINO_RASPBERRY_PI_PICO)

    #define SerialNCP Serial1

    unsigned long baud = 74880;

    void ncpInit() {
      SerialNCP.begin(baud);
    }

    #include "loop_baud_check.h"

#elif defined(SEEED_WIO_TERMINAL)

    #include "TFT_eSPI.h"

    #define SerialNCP RTL8720D

    unsigned long baud = 115200;

    TFT_eSPI tft;

    void ncpInit() {
      tft.begin();
      tft.setRotation(3);
      tft.fillScreen(TFT_BLACK);

      tft.setFreeFont(&FreeSansBoldOblique12pt7b);
      tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
      tft.setTextDatum(BC_DATUM);
      tft.drawString("Flashing RTL8720...", TFT_HEIGHT/2, TFT_WIDTH/2);

      // initialize both serial ports:
      pinMode(PIN_SERIAL2_RX, OUTPUT);
      pinMode(RTL8720D_CHIP_PU, OUTPUT);
      digitalWrite(PIN_SERIAL2_RX, LOW);
      digitalWrite(RTL8720D_CHIP_PU, LOW);
      delay(100);
      digitalWrite(RTL8720D_CHIP_PU, HIGH);
      delay(500);
      pinMode(PIN_SERIAL2_RX, INPUT);

      //Serial.beginWithoutDTR(baud);

      SerialNCP.begin(baud);

      //delay(500);
    }

    #include "loop_baud_check.h"

#else
    #error "Platform not supported"
#endif

/*
 * LED
 */

#if !defined(LED_PIN) && defined(LED_BUILTIN)
  #define LED_PIN LED_BUILTIN
#endif

void ledInit() {
#if defined(LED_PIN)
  pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, HIGH);
#endif
}

void ledOFF() {
  digitalWrite(LED_PIN, HIGH);
}

void ledToggle() {
  uint32_t now = millis();
  static uint32_t last_toggle = now;
  if (now - last_toggle > 100) {
    last_toggle = now;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}


/*
 * Main
 */

void setup() {
  SerialPC.begin(baud);

  ledInit();
  ncpInit();
}

