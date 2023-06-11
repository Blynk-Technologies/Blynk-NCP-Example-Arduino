/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial

#include <BlynkNcpClient.h>
#include <DemoHelpers.h>

BlynkTimer timer;

BLYNK_CONNECTED() {
  BLYNK_LOG("Connected to Blynk 🙌");
}

BLYNK_DISCONNECTED() {
  BLYNK_LOG("Blynk disconnected");
}

BLYNK_WRITE(V1) {
  int value = BlynkMathClamp(param.asInt(), 0, 63);

  char buff[64];
  memset(buff, '|', sizeof(buff));
  if (value) {
    buff[value] = '\0';
  } else {
    strcpy(buff, " ");
  }

  BLYNK_LOG("Got: %s", buff);

  Blynk.virtualWrite(V2, buff);
  Blynk.setProperty(V2, "color", valueToColor(value));
}

void setup() {
  Serial.begin(115200);
  waitSerialConsole(Serial);
  Serial.println();

  Blynk.onStateChange([]() {
    BLYNK_LOG("State: %s", Blynk.getStateString());
  });

  if (!Blynk.begin(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME)) {
    BLYNK_LOG("Blynk.NCP initialization failed!");
    while (true) { delay(100); }
  }

  timer.setInterval(1000, []() {
    Blynk.virtualWrite(V0, millis());
  });
}

void loop() {
  timer.run();
  Blynk.run();
}
