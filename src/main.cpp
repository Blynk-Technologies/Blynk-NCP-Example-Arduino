/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

/* The firmware version of the Primary MCU (used for OTA updates) */
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// Debug output
#define BLYNK_PRINT                   Serial

#include <OneButton.h>
#include <ArduinoUtils.h>
#include <BoardSetup.h>
#include <BlynkEdgentNCP.h>

BlynkTimer timer;

BLYNK_CONNECTED() {
  BLYNK_LOG("Connected to Blynk 🙌");
}

BLYNK_DISCONNECTED() {
  BLYNK_LOG("Blynk disconnected");
}

BLYNK_WRITE(V1) {
  uint32_t color = rainbow(param.asInt(), 0, 64);
  Blynk.setProperty(V0, "color", RGBtoHEX(color));
  displayColor(color);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  BoardSetup();

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
    displayMessage(Blynk.getStateString());
  });

  // Set config mode timeout to 30 minutes, for testing purposes
  Blynk.setConfigTimeout(30*60);

  // Set up the user button
  button.setLongPressIntervalMs(1000);

  button.attachClick([]() {
    displayMessage("Click!");
    timer.setTimeout(1000, displayClear);
  });
  button.attachDoubleClick([]() {
    displayMessage("Double Click!");
    timer.setTimeout(1000, displayClear);
  });

  button.attachLongPressStart([]() {
    displayMessage("Hold button to reset config");
  });
  button.attachDuringLongPress([]() {
    const uint32_t t = button.getPressedMs();
    if (t > 10000 && t < 15000) {
      displayMessage("Release button");
    } else if (t > 15000) {
      displayClear();
    }
  });
  button.attachLongPressStop([]() {
    const uint32_t t = button.getPressedMs();
    if (t > 10000 && t < 15000) {
      if (Blynk.resetConfig()) {
        BLYNK_LOG("Blynk.NCP configuration is erased");
      }
    }
    displayClear();
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
