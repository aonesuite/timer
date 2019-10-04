#include "Arduino.h"
#include "Manager.h"

Manager m;

#define BAUD 9600

void setup()
{
  Serial.begin(BAUD);
  m = Manager();
}

void loop()
{
  m.loop();
}