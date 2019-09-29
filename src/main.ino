#include "Arduino.h"
#include "Manager.h"

Manager m;

void setup()
{
  Serial.begin(9600);
  m = Manager();
}

void loop()
{
  m.loop();
}