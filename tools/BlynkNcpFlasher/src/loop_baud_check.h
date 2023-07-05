
void loop() {
  // check if the USB virtual serial wants a new baud rate
  if (SerialPC.baud() != baud) {
    baud = SerialPC.baud();
    SerialNCP.begin(baud);
  }
  #include "passthrough.h"
}
