
void loop() {

  const auto _rts = SerialPC.rts();
  const auto _dtr = SerialPC.dtr();

  if ((rts != _rts) || (dtr != _dtr)) {
    rts = _rts;
    dtr = _dtr;
#ifdef NCP_PIN_RESETN
    digitalWrite(NCP_PIN_RESETN, _rts ? LOW : HIGH);
#endif
#ifdef NCP_PIN_GPIO0
    digitalWrite(NCP_PIN_GPIO0, _dtr ? LOW : HIGH);
#endif

    ledOFF();
  }

  int len = 0;
  static uint8_t buffer[488];
  while (SerialPC.available() && len < sizeof(buffer)) {
    buffer[len++] = SerialPC.read();
  }
  if (len) {
    ledToggle();
    SerialNCP.write(buffer, len);
  }

  len = 0;
  while (SerialNCP.available() && len < sizeof(buffer)) {
    buffer[len++] = SerialNCP.read();
  }
  if (len) {
    ledToggle();
    SerialPC.write(buffer, len);
  }

  // check if the USB virtual serial wants a new baud rate
  if (SerialPC.baud() != baud) {
    //rts = -1;
    //dtr = -1;

    baud = SerialPC.baud();
    SerialNCP.end();
    SerialNCP.begin(baud);
  }
}

