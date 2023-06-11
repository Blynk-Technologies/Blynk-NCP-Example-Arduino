
template <typename T>
void waitSerialConsole(T& ser) {
  // Wait for serial console, up to 3 seconds
  const uint32_t tstart = millis();
  while (!ser && (millis() - tstart < 2900)) { delay(1); }
  delay(100);
}

static
String valueToColor(int val) {
  int rgbValue = map(val, 0, 63, 0, 1535);
  int red;
  int blue;
  int green;

  if (rgbValue < 256) {
    red = 255;
    blue = rgbValue;
    green = 0;
  }
  else if (rgbValue < 512) {
    red = 511 - rgbValue;
    blue = 255;
    green = 0;
  }
  else if (rgbValue < 768) {
    red = 0;
    blue = 255;
    green = rgbValue - 512;
  }
  else if (rgbValue < 1024) {
    red = 0;
    blue = 1023 - rgbValue;
    green = 255;
  }
  else if (rgbValue < 1280) {
    red = rgbValue - 1024;
    blue = 0;
    green = 255;
  }
  else {
    red = 255;
    blue = 0;
    green = 1535 - rgbValue;
  }

  char buff[32];
  sprintf(buff, "#%02x%02x%02x", red, green, blue);
  return buff;
}

#if defined(LINUX)
int main(int argc, char* argv[])
{
  setup();
  while (true) {
    loop();
    delay(1);
  }
  return 0;
}
#endif
