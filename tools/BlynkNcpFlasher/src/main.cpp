#include <Arduino.h>

#if defined(ARDUINO_NANO_RP2040_CONNECT) || \
    defined(ARDUINO_SAMD_NANO_33_IOT)    || \
    defined(ARDUINO_SAMD_MKRWIFI1010)

#define SerialNCP SerialNina

unsigned long baud = 115200;
int rts = -1;
int dtr = -1;

void ncpInit() {
  SerialNCP.begin(baud);

  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);
}

void ncpRun() {
  if (rts != Serial.rts()) {
    rts = Serial.rts();
#if defined(ARDUINO_SAMD_NANO_33_IOT)
    digitalWrite(NINA_RESETN, rts ? LOW : HIGH);
#else
    digitalWrite(NINA_RESETN, rts ? HIGH : LOW);
#endif
  }

  if (dtr != Serial.dtr()) {
    dtr = Serial.dtr();
    digitalWrite(NINA_GPIO0, (dtr == 0) ? HIGH : LOW);
  }

  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    rts = -1;
    dtr = -1;

    baud = Serial.baud();
    SerialNCP.begin(baud);
  }
}

#elif defined(ARDUINO_AVR_UNO_WIFI_REV2)

#define SerialNCP SerialNina

unsigned long baud = 115200;

void ncpInit() {
  SerialNCP.begin(baud);

  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);

  // Manually switch NINA module into upload mode
  digitalWrite(NINA_GPIO0, LOW);

  digitalWrite(NINA_RESETN, LOW);
  delay(100);
  digitalWrite(NINA_RESETN, HIGH);
  delay(100);
  digitalWrite(NINA_RESETN, LOW);
}

void ncpRun() {
}

#elif defined(ARDUINO_SAMD_MKRVIDOR4000)

#include <VidorPeripherals.h>

#define SerialNCP SerialNina

unsigned long baud = 119400;
int rts = -1;
int dtr = -1;

void ncpInit() {
  FPGA.begin();
  SerialNCP.begin(baud);

  FPGA.pinMode(FPGA_NINA_GPIO0, OUTPUT);
  FPGA.pinMode(FPGA_SPIWIFI_RESET, OUTPUT);
}

void ncpRun() {
  if (rts != Serial.rts()) {
    rts = Serial.rts();
    FPGA.digitalWrite(FPGA_SPIWIFI_RESET, (rts == 1) ? LOW : HIGH);
  }

  if (dtr != Serial.dtr()) {
    dtr = Serial.dtr();
    FPGA.digitalWrite(FPGA_NINA_GPIO0, (dtr == 1) ? HIGH : LOW);
  }

  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    rts = -1;
    dtr = -1;

    baud = Serial.baud();
  }
}

#elif defined(ARDUINO_RASPBERRY_PI_PICO)

#define SerialNCP Serial1

unsigned long baud = 74880;

void ncpInit() {
  SerialNCP.begin(baud);
}

void ncpRun() {
  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    baud = Serial.baud();
    SerialNCP.begin(baud);
  }
}

#else
  #error "Platform not supported"
#endif

/*
 * LED
 */

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
  uint32_t now = millis();
  static uint32_t last_toggle = now;
  if (now - last_toggle > 100) {
    last_toggle = now;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}


/*
 * Main
 */

void setup() {
  Serial.begin(baud);

  ledInit();
  ncpInit();
}

void loop() {
  ncpRun();

  static char buffer[512];

  if (Serial.available() > 0) {
    unsigned len = min(Serial.available(), sizeof(buffer));
    Serial.readBytes(buffer, len);
    SerialNCP.write(buffer, len);
    ledToggle();
  }

  if (SerialNCP.available() > 0) {
    unsigned len = min(SerialNCP.available(), sizeof(buffer));
    SerialNCP.readBytes(buffer, len);
    Serial.write(buffer, len);
    ledToggle();
  }
}
