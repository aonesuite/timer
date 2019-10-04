#ifndef TICKER_H
#define TICKER_H

void initISR();
void loopISR();
uint64_t getISRTimeCount();
void setISRTimeCount(uint64_t seconds);

#endif
