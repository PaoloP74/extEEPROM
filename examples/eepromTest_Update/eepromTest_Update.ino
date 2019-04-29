// Test extEEPROM library - update() method.
// Damian Wrobel <dwrobel@ertelnet.rybnik.pl>

#include <extEEPROM.h>

extEEPROM ep(kbits_256, 1, 64); // device size, number of devices, page size

void test_update() {
  Serial.println("\n(0) Test Started");

  constexpr auto addr  = 0;
  constexpr int length = (BUFFER_LENGTH * 2) + 13;

  byte writeBuf[length];

  for (auto i = 0; i < length; i++) {
    writeBuf[i] = i;
  }

  auto rv = ep.update(addr, writeBuf, length);
  if (rv) {
    Serial.print("(1) Error updating: ");
    Serial.println(rv);
  }

  byte readBuf[length];
  for (auto i = 0; i < length; i++) {
    readBuf[i] = ~writeBuf[i];
  }

  rv = ep.read(addr, readBuf, length);
  if (rv) {
    Serial.print("(2) Error reading: ");
    Serial.println(rv);
  }

  rv = memcmp(writeBuf, readBuf, length);
  if (rv) {
    Serial.print("(3) Error comparing: ");
    Serial.println(rv);
  }

  for (auto i = 0; i < length; i++) {
    if (i % 2 || i % 3)
      writeBuf[i] = i;
  }

  rv = ep.update(addr, writeBuf, length);
  if (rv) {
    Serial.print("(4) Error updating: ");
    Serial.println(rv);
  }

  rv = ep.read(addr, readBuf, length);
  if (rv) {
    Serial.print("(5) Error reading: ");
    Serial.println(rv);
  }

  rv = memcmp(writeBuf, readBuf, length);
  if (rv) {
    Serial.print("(6) Error comparing: ");
    Serial.println(rv);
  }

  Serial.println("(7) Test OK");
}

void setup(void) {
  Serial.begin(115200);

  const auto eepStatus = ep.begin(ep.twiClock100kHz);
  if (eepStatus) {
    Serial.print("extEEPROM.begin() failed, status = ");
    Serial.println(eepStatus);
    while (1)
      ;
  }

  test_update();
}

void loop(void) {
}
