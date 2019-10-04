#ifndef TICKER_H
#define TICKER_H

#include "Arduino.h"

void initISR();
void loopISR();
uint64_t getISRTimeCount();
void setISRTimeCount(uint64_t seconds);

#endif
