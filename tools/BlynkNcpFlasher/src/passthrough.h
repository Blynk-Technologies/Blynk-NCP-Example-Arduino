
  static char buffer[512];

  if (SerialPC.available() > 0) {
    unsigned len = min(SerialPC.available(), sizeof(buffer));
    SerialPC.readBytes(buffer, len);
    SerialNCP.write(buffer, len);
    ledToggle();
  }

  if (SerialNCP.available() > 0) {
    unsigned len = min(SerialNCP.available(), sizeof(buffer));
    SerialNCP.readBytes(buffer, len);
    SerialPC.write(buffer, len);
    ledToggle();
  }

