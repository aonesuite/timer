
#include <string.h>

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;

//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 3;

//Pin connected to Data in (DS) of 74HC595
const int dataPin = 9;

unsigned char CHAR_MAP[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf, 0xff, 0x86, 0x8E };
char CHAR_INDEX_MAP[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', ' ', 'E', 'F' };

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
}

// 点亮数码管
void ledshow(const char* str) {
  digitalWrite(latchPin, LOW);

  int len = strlen(str);
  int hasDot = 0;
  for (int i = len - 1, m = 0; i >= 0; i--) {
    char chr = str[i];
    if (chr == '.') {
      hasDot = 1;
      continue;
    }
    int n = 0;
    for (; n < 17; n++) {
      if (chr == CHAR_INDEX_MAP[n])
        break;
    }
    if (n != 17) {
      unsigned char chr1 = CHAR_MAP[n];
      if (hasDot) chr1 &= 0x7f; // 处理小数点
      shiftOut(dataPin, clockPin, MSBFIRST, chr1);
    }

    m++;
    hasDot = 0;
  }

  digitalWrite(latchPin, HIGH);
}

void loop() {
  ledshow("05 40.129");
}
