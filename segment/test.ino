#include <Arduino.h>

#define SER 13
#define CLK 12
#define ENABLE 8

void setup() {
    pinMode(SER, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, LOW);
    shiftOut(SER, CLK, MSBFIRST,0x00);
    shiftOut(SER, CLK, MSBFIRST,0xFF);
    digitalWrite(ENABLE, HIGH);
}

void loop() {
}