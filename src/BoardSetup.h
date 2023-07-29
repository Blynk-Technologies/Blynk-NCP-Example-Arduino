#pragma once

#if defined(ARDUINO_TTGO_T_PICO_C3)

  #include <TFT_eSPI.h>

  #include "hardware/gpio.h"
  #include "hardware/adc.h"

  #include "BlynkLogoBlack96.h"
  #include "NotoSansBold15.h"

  TFT_eSPI tft;
  OneButton button(PIN_BUTTON1, true);
  OneButton button2(PIN_BUTTON2, true);

  static
  void BoardSetup() {
    /* Open the power supply */
    pinMode(PIN_PWR_ON, OUTPUT);
    digitalWrite(PIN_PWR_ON, HIGH);

    /* Configure battery voltage detection pin */
    adc_init();
    adc_gpio_init(PIN_BAT_VOLT);
    adc_select_input(0);

    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, 0);

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
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(text, TFT_HEIGHT/2, TFT_WIDTH/2);
  }

  static
  void displayColor(uint32_t color) {
    tft.fillSmoothRoundRect(10, 10, TFT_HEIGHT-20, TFT_WIDTH-20, 7,
                            RGBtoRGB16(color), TFT_BLACK);
  }


#else

  // Attach a momentary push button to pin 6 (active LOW)
  OneButton button(6, true);

  static
  void BoardSetup() {
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
    Serial.printf("Color: %06x\n", color);
  }

#endif
