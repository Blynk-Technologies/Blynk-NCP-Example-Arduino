#pragma once

#if !defined(__AVR__)
  // define std::min, std::max before Arduino
  #include <algorithm>
#endif

#include <Arduino.h>
#include <stdio.h>

// Create Serial1 for ARDUINO_AVR_UNO and similar boards
#if defined(__AVR_ATmega328P__)
  #include <SoftwareSerial.h>
  SoftwareSerial Serial1(BLYNK_NCP_RX, BLYNK_NCP_TX);
#endif

// Wait for serial console, up to 3 seconds
template <typename T>
void waitSerialConsole(T& ser) {
#if defined(LINUX)
  // Not needed on linux
  (void) ser;
#elif defined(ARDUINO_UNOWIFIR4)
  delay(1500);
#else
  while (!ser && (millis() < 3000)) { delay(10); }
  delay(100);
#endif
}

static inline
uint32_t rainbow(int val, int val_min, int val_max)
{
  const int hue = map(val, val_min, val_max, 0, 1535);
  long red, green, blue;

  if (hue < 256) {
    red = 255;
    blue = hue;
    green = 0;
  }
  else if (hue < 512) {
    red = 511 - hue;
    blue = 255;
    green = 0;
  }
  else if (hue < 768) {
    red = 0;
    blue = 255;
    green = hue - 512;
  }
  else if (hue < 1024) {
    red = 0;
    blue = 1023 - hue;
    green = 255;
  }
  else if (hue < 1280) {
    red = hue - 1024;
    blue = 0;
    green = 255;
  }
  else {
    red = 255;
    blue = 0;
    green = 1535 - hue;
  }
  return (red << 16) | (green << 8) | blue;
}

static inline
String RGBtoHEX(uint32_t rgb32) {
  char buff[32];
  snprintf(buff, sizeof(buff),
           "#%02" PRIx32 "%02" PRIx32 "%02" PRIx32,
           (rgb32>>16)&0xFF, (rgb32>>8)&0xFF, rgb32&0xFF);
  return buff;
}

static inline
uint16_t RGBtoRGB16(uint32_t rgb32) {
  return (rgb32>>8&0xf800) | (rgb32>>5&0x07e0) | (rgb32>>3&0x001f);
}

// Entry point for Linux target
#if defined(LINUX)
int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;

  setup();
  while (true) {
    loop();
    delay(1);
  }
  return 0;
}
#endif
