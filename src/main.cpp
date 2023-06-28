/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

/* The firmware version of the Primary MCU (used for OTA updates) */
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// Debug output
#define BLYNK_PRINT                   Serial

// Redefine NCP type and connection port settings, if needed
//#define BLYNK_NCP_SERIAL            Serial1
//#define BLYNK_NCP_BAUD              2000000
//#define BLYNK_NCP_MACCHINA_SUPERB

#include <BlynkEdgentNCP.h>
#include <ArduinoUtils.h>

BlynkTimer timer;

BLYNK_CONNECTED() {
  BLYNK_LOG("Connected to Blynk 🙌");
}

BLYNK_DISCONNECTED() {
  BLYNK_LOG("Blynk disconnected");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  waitSerialConsole(Serial);

  BLYNK_LOG("Main firmware: %s", BLYNK_FIRMWARE_VERSION);

  // Initialize the Blynk.NCP hardware
  if (Blynk.initNCP()) {
    String ver = Blynk.getNcpVersion();
    BLYNK_LOG("Blynk.NCP firmware: %s", ver.c_str());
  } else {
    BLYNK_LOG("Cannot communicate to Blynk.NCP");
    return;
  }

  // Print state changes
  Blynk.onStateChange([]() {
    BLYNK_LOG("State: %s", Blynk.getStateString());
  });

  // White labeling
  //Blynk.setVendorPrefix("MyCompany");
  //Blynk.setVendorServer("dashboard.mycompany.com");
  // Product setup
  Blynk.begin(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME);

  // Publish some data periodically
  timer.setInterval(1000, []() {
    Blynk.virtualWrite(V0, millis());
  });
}

void loop() {
  timer.run();
  Blynk.run();
}
