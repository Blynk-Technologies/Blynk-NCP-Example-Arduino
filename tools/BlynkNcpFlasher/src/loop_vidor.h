
void loop() {
  if (rts != SerialPC.rts()) {
    rts = SerialPC.rts();
    FPGA.digitalWrite(FPGA_SPIWIFI_RESET, (rts == 1) ? LOW : HIGH);
  }

  if (dtr != SerialPC.dtr()) {
    dtr = SerialPC.dtr();
    FPGA.digitalWrite(FPGA_NINA_GPIO0, (dtr == 1) ? HIGH : LOW);
  }

  // check if the USB virtual serial wants a new baud rate
  if (SerialPC.baud() != baud) {
    rts = -1;
    dtr = -1;

    baud = SerialPC.baud();
  }

  #include "passthrough.h"
}
