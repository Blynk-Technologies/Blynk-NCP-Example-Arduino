
unsigned long baud = 9600;

int rts = -1;
int dtr = -1;


#ifdef ARDUINO_PORTENTA_C33

  #ifndef SerialNCP
  #define SerialNCP       Serial5
  #endif

  #ifndef NINA_GPIO0
  #define NINA_GPIO0      (100)
  #endif

  #ifndef NINA_RESETN
  #define NINA_RESETN     (101)
  #endif

#else

  #ifndef SerialNCP
  #define SerialNCP       Serial2
  #endif

  #ifndef NINA_GPIO0
  #define NINA_GPIO0      (28)
  #endif

  #ifndef NINA_RESETN
  #define NINA_RESETN     (29)
  #endif

#endif

void ledsOFF() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void setup() {
  Serial.begin(baud, SERIAL_8N1);
  SerialNCP.begin(baud);
  while (!Serial);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  ledsOFF();

  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);

  digitalWrite(NINA_GPIO0, HIGH);
  delay(100);
  digitalWrite(NINA_RESETN, HIGH);
  digitalWrite(NINA_RESETN, LOW);
  digitalWrite(NINA_RESETN, HIGH);

  rts = Serial.rts();
  dtr = Serial.dtr();
}

void loop() {

  auto _rts = Serial.rts();
  auto _dtr = Serial.dtr();

  if ((rts != _rts) || (dtr != _dtr)) {
    digitalWrite(NINA_RESETN, _rts ? LOW : HIGH);
    rts = _rts;
    digitalWrite(NINA_GPIO0, _dtr ? LOW : HIGH);
    dtr = _dtr;

    ledsOFF();
  }

  int len = 0;
  static uint8_t buffer[488];
  while (Serial.available() && len < sizeof(buffer)) {
    buffer[len++] = Serial.read();
  }
  if (len) {
    digitalWrite(LEDB, !digitalRead(LEDB));
    SerialNCP.write(buffer, len);
  }

  len = 0;
  while (SerialNCP.available() && len < sizeof(buffer)) {
    buffer[len++] = SerialNCP.read();
  }
  if (len) {
    digitalWrite(LEDB, !digitalRead(LEDB));
    Serial.write(buffer, len);
  }

  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    //rts = -1;
    //dtr = -1;

    baud = Serial.baud();
    SerialNCP.end();
    SerialNCP.begin(baud);
  }
}
