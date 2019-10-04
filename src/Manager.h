#ifndef MANAGER_H
#define MANAGER_H

#define MODE_CLOCK 0
#define MODE_COUNTDOWN 1

#include "Clock.h"
#include "Display.h"
#include "config.h"

class Manager
{
public:
  Manager();
  void loop();

private:
  volatile int mode;
  Display *display;
  Clock *clock;
};

#endif
