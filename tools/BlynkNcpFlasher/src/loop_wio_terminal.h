
void loop() {
  // check if the USB virtual serial wants a new baud rate
  if (SerialPC.baud() != baud) {
    baud = SerialPC.baud();
    SerialNCP.begin(baud);
    if (baud == 2400) {
      ncpReboot();
    }
  }
  #include "passthrough.h"
}
