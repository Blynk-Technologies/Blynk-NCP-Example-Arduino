#pragma once

#if defined(ARDUINO_TTGO_T_PICO_C3)

  #include <TFT_eSPI.h>

  #include "BlynkLogoBlack96.h"
  #include "NotoSansBold15.h"

  TFT_eSPI tft;
  OneButton button(PIN_BUTTON1, true);
  OneButton button2(PIN_BUTTON2, true);

  static
  void setupBoard() {
    /* Open the power supply */
    pinMode(PIN_PWR_ON, OUTPUT);
    digitalWrite(PIN_PWR_ON, HIGH);

    tft.begin();
    tft.initDMA();
    tft.setSwapBytes(true);
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.pushImage((TFT_HEIGHT-96)/2,
                  (TFT_WIDTH-96)/2,
                  96, 96,
                  BlynkLogoBlack96);

    /* Gradually lighten up */
    pinMode(PIN_TFT_BL, OUTPUT);
    for (int i = 0; i <= 120; i += 5) {
      analogWrite(PIN_TFT_BL, i);
      delay(20);
    }
    delay(1000);

    tft.fillScreen(TFT_BLACK);
    tft.loadFont(NotoSansBold15);
    tft.setTextWrap(true, true);
  }

  static
  void displayClear() {
    tft.fillScreen(TFT_BLACK);
  }

  static
  void displayMessage(const String& text) {
    Serial.println(text);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(text, TFT_HEIGHT/2, TFT_WIDTH/2);
  }

  static
  void displayColor(uint32_t color) {
    Serial.printf("Color: %06" PRIx32 "\n", color);
    tft.fillSmoothRoundRect(10, 10, TFT_HEIGHT-20, TFT_WIDTH-20, 7,
                            RGBtoRGB16(color), TFT_BLACK);
  }

#elif defined(SEEED_WIO_TERMINAL)

  #include <TFT_eSPI.h>
  #include "WioTerminalBacklight.h"
  #include "BlynkLogoBlack96.h"
  #include "NotoSansBold15.h"

  TFT_eSPI tft;
  LCDBackLight backLight;

  OneButton button(WIO_KEY_C, true);
  OneButton button2(WIO_KEY_B, true);
  OneButton button3(WIO_KEY_A, true);

  static
  void setupBoard() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    tft.begin();
    tft.setSwapBytes(true);
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.pushImage((TFT_HEIGHT-96)/2,
                  (TFT_WIDTH-96)/2,
                  96, 96,
                  BlynkLogoBlack96);

    /* Gradually lighten up */
    backLight.initialize();
    for (int i = 0; i <= backLight.getMaxBrightness()/2; i++) {
      backLight.setBrightness(i);
      delay(20);
    }

    delay(1000);

    tft.fillScreen(TFT_BLACK);
    tft.loadFont(NotoSansBold15);
    tft.setTextWrap(true, true);
  }

  static
  void displayClear() {
    tft.fillScreen(TFT_BLACK);
  }

  static
  void displayMessage(const String& text) {
    Serial.println(text);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(text, TFT_HEIGHT/2, TFT_WIDTH/2);
  }

  static
  void displayColor(uint32_t color) {
    Serial.printf("Color: %06" PRIx32 "\n", color);
    tft.fillSmoothRoundRect(20, 20, TFT_HEIGHT-40, TFT_WIDTH-40, 7,
                            RGBtoRGB16(color), TFT_BLACK);
  }

#else

  // Attach a momentary push button to pin 6 (active LOW)
  OneButton button(6, true);

  static
  void setupBoard() {
  }

  static
  void displayClear() {
  }

  static
  void displayMessage(const String& text) {
    Serial.println(text);
  }

  static
  void displayColor(uint32_t color) {
    char buff[32];
    snprintf(buff, sizeof(buff),
             "Color: %06" PRIx32, color);
    Serial.println(buff);
  }

#endif
