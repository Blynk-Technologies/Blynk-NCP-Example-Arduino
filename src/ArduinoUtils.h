
template <typename T>
void waitSerialConsole(T& ser) {
  // Wait for serial console, up to 3 seconds
  const uint32_t tstart = millis();
  while (!ser && (millis() - tstart < 2900)) { delay(1); }
  delay(100);
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
