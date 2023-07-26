/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

/* The firmware version of the Primary MCU (used for OTA updates) */
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// Debug output
#define BLYNK_PRINT                   Serial

#include <ArduinoUtils.h>
#include <BlynkEdgentNCP.h>
#include <OneButton.h>

BlynkTimer timer;

// Attach a momentary push button to pin 6 (active LOW)
OneButton button(6, true);

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
  BLYNK_LOG("Build: %s", __DATE__ " " __TIME__);

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

  // Set config mode timeout to 30 minutes, for testing purposes
  Blynk.setConfigTimeout(30*60);

  // Set up the user button
  button.setLongPressIntervalMs(1000);
  button.attachLongPressStart([]() {
    BLYNK_LOG("Hold button for 10s to reset config");
  });
  button.attachDuringLongPress([]() {
    const uint32_t t = button.getPressedMs();
    if (t > 10000 && t < 15000) {
      BLYNK_LOG("Release button to reset config");
    }
  });
  button.attachLongPressStop([]() {
    const uint32_t t = button.getPressedMs();
    if (t > 10000 && t < 15000) {
      Blynk.resetConfig();
      BLYNK_LOG("Blynk.NCP configuration is erased");
    }
  });

  // White labeling (use this ONLY if you have a branded Blynk App)
  //Blynk.setVendorPrefix("MyCompany");
  //Blynk.setVendorServer("dashboard.mycompany.com");

  // Product setup
  Blynk.begin(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME);

  // Publish some data periodically
  timer.setInterval(1000, []() {
    Blynk.virtualWrite(V0, millis() / 1000);
  });
}

void loop() {
  timer.run();
  Blynk.run();
  button.tick();
  delay(1);
}
