/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// Debug output
#define BLYNK_PRINT                   Serial

// Redefine NCP type and connection port settings, if needed
//#define BLYNK_NCP_SERIAL            Serial1
//#define BLYNK_NCP_BAUD              2000000
//#define BLYNK_NCP_MACCHINA_SUPERB

#include <BlynkNcpClient.h>

BlynkTimer timer;

BLYNK_CONNECTED() {
  BLYNK_LOG("Connected to Blynk 🙌");
}

BLYNK_DISCONNECTED() {
  BLYNK_LOG("Blynk disconnected");
}

template <typename T>
void waitSerialConsole(T& ser) {
  // Wait for serial console, up to 3 seconds
  const uint32_t tstart = millis();
  while (!ser && (millis() - tstart < 2900)) { delay(1); }
  delay(100);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  waitSerialConsole(Serial);

  if (Blynk.initNCP()) {
    String ver = Blynk.getNcpVersion();
    BLYNK_LOG("Blynk.NCP firmware: %s", ver.c_str());
  } else {
    BLYNK_LOG("Cannot communicate to Blynk.NCP");
    return;
  }

  Blynk.onStateChange([]() {
    BLYNK_LOG("State: %s", Blynk.getStateString());
  });

  Blynk.begin(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME);

  timer.setInterval(1000, []() {
    Blynk.virtualWrite(V0, millis());
  });
}

void loop() {
  timer.run();
  Blynk.run();
}
