
void loop() {
  if (rts != SerialPC.rts()) {
    rts = SerialPC.rts();
#if defined(ARDUINO_SAMD_NANO_33_IOT)
    digitalWrite(NINA_RESETN, rts ? LOW : HIGH);
#else
    digitalWrite(NINA_RESETN, rts ? HIGH : LOW);
#endif
  }

  if (dtr != SerialPC.dtr()) {
    dtr = SerialPC.dtr();
    digitalWrite(NINA_GPIO0, (dtr == 0) ? HIGH : LOW);
  }

  // check if the USB virtual serial wants a new baud rate
  if (SerialPC.baud() != baud) {
    rts = -1;
    dtr = -1;

    baud = SerialPC.baud();
    SerialNCP.begin(baud);
  }
  #include "passthrough.h"
}
