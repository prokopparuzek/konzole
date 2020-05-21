#include <Arduino.h>

#define SER 13
#define CLK 8
#define ENABLE 12


void setup() {
    pinMode(SER, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, LOW);
    shiftOut(SER, CLK, MSBFIRST,0x00);
    shiftOut(SER, CLK, MSBFIRST,0xF0);
    digitalWrite(ENABLE, HIGH);
}

void loop() {
}