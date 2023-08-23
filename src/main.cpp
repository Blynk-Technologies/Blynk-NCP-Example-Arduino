/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

/* The firmware version of the Primary MCU (used for OTA updates) */
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// Debug output
#define BLYNK_PRINT                   Serial

#include <ArduinoUtils.h>
#include <OneButton.h>
#include <BoardSetup.h>
#include <BlynkEdgentNCP.h>

BlynkTimer timer;

BLYNK_CONNECTED() {
  BLYNK_LOG("Connected to Blynk 🙌");
  Blynk.syncVirtual(V1);
}

BLYNK_DISCONNECTED() {
  BLYNK_LOG("Blynk disconnected");
}

BLYNK_WRITE(V1) {
  uint32_t color = rainbow(param.asInt(), 0, 64);
  Blynk.setProperty(V0, "color", RGBtoHEX(color));
  displayColor(color);
}

void setupUserButton() {
  Button1.setLongPressIntervalMs(1000);

  Button1.attachClick([]() {
    displayMessage("Click!");
    timer.setTimeout(1000, displayClear);
  });
  Button1.attachDoubleClick([]() {
    displayMessage("Double Click!");
    timer.setTimeout(1000, displayClear);
  });

  Button1.attachLongPressStart([]() {
    displayMessage("Hold button to reset config");
  });
  Button1.attachDuringLongPress([]() {
    const uint32_t t = Button1.getPressedMs();
    if (t > 10000 && t < 15000) {
      displayMessage("Release button");
    } else if (t > 15000) {
      displayClear();
    }
  });
  Button1.attachLongPressStop([]() {
    const uint32_t t = Button1.getPressedMs();
    if (t > 10000 && t < 15000) {
      if (Blynk.resetConfig()) {
        BLYNK_LOG("Blynk.NCP configuration is erased");
      }
    }
    displayClear();
  });
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  setupBoard();
  setupUserButton();
  waitSerialConsole(Serial);

  BLYNK_LOG("Main firmware: %s", BLYNK_FIRMWARE_VERSION);
  BLYNK_LOG("Build: %s", __DATE__ " " __TIME__);

  displayMessage("Initializing Blynk.NCP");

  if (Blynk.initNCP()) {
    String ver = Blynk.getNcpVersion();
    BLYNK_LOG("Blynk.NCP firmware: %s", ver.c_str());
  } else {
    displayMessage("Cannot communicate to Blynk.NCP");
    return;
  }

  // Print state changes
  Blynk.onStateChange([]() {
    displayMessage(Blynk.getStateString());
  });

  // Set config mode timeout to 30 minutes, for testing purposes
  Blynk.setConfigTimeout(30*60);

  // White labeling (use this ONLY if you have a branded Blynk App)
  //Blynk.setVendorPrefix("MyCompany");
  //Blynk.setVendorServer("dashboard.mycompany.com");

  // Product setup
  if (!Blynk.begin(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME)) {
    displayMessage("Error. Invalid Template ID?");
    return;
  }

  // Publish some data periodically
  timer.setInterval(1000, []() {
    Blynk.virtualWrite(V0, millis() / 1000);
  });
}

void loop() {
  timer.run();
  Blynk.run();
  Button1.tick();
  delay(1);
}
