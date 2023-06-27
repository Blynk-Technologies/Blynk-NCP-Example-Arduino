
long baud = 9600;
int rts = -1;
int dtr = -1;

#ifdef SerialPC
  // Good
#elif defined(ARDUINO_UNOWIFIR4)
  #define SerialPC        SerialUSB
#else
  #define SerialPC        Serial
#endif

#ifdef SerialNCP
  // Good
  void ncpInit() {}
#elif defined(ARDUINO_UNOWIFIR4)
  #define SerialNCP       Serial2
  void ncpInit() {}
#elif defined(ARDUINO_UNOR4_MINIMA)
  #define SerialNCP       Serial1
  void ncpInit() {}
#elif defined(ARDUINO_PORTENTA_C33)
  #define SerialNCP       Serial5
  #define NCP_PIN_GPIO0   (100)
  #define NCP_PIN_RESETN  (101)
  #define LED_PIN         LEDB
  void ncpInit() {
    pinMode(LEDR, OUTPUT); digitalWrite(LEDR, HIGH);
    pinMode(LEDG, OUTPUT); digitalWrite(LEDG, HIGH);

    pinMode(NCP_PIN_GPIO0, OUTPUT);
    pinMode(NCP_PIN_RESETN, OUTPUT);

    digitalWrite(NCP_PIN_GPIO0, HIGH);
    delay(100);
    digitalWrite(NCP_PIN_RESETN, HIGH);
    digitalWrite(NCP_PIN_RESETN, LOW);
    digitalWrite(NCP_PIN_RESETN, HIGH);
  }
#endif

#if !defined(LED_PIN) && defined(LED_BUILTIN)
  #define LED_PIN LED_BUILTIN
#endif

void ledInit() {
#if defined(LED_PIN)
  pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, HIGH);
#endif
}

void ledOFF() {
  digitalWrite(LED_PIN, HIGH);
}

void ledToggle() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void setup() {
  SerialPC.begin(baud, SERIAL_8N1);
  SerialNCP.begin(baud);
  while (!SerialPC);

  ledInit();
  ncpInit();

  rts = SerialPC.rts();
  dtr = SerialPC.dtr();
}

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

