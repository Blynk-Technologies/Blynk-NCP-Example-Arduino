#include <Arduino.h>

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
#else
  const uint32_t tstart = millis();
  while (!ser && (millis() - tstart < 2900)) { delay(1); }
  delay(100);
#endif
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
