#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "music/tetris.h"

#define RED 11
#define YELLOW 10
#define GREEN 9

#define LEFT 7
#define RIGHT 6

#define BEEP 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

int note = 0;
unsigned long int timer;
uint16_t fr;
uint16_t length;
byte octave = 1;

void setup() {
  // LED setup
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  // button setup
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  // beep setup
  pinMode(BEEP, OUTPUT);
  // lcd setup
  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print(F("Omen Charlie"));
  // Serial setup
  Serial.begin(9600);
  Serial.println(F("test"));
  // start play music
  fr = pgm_read_word_near(song);
  timer = millis();
  tone(BEEP, fr);
}

void loop() {
  sound();  // play music
  if (digitalRead(LEFT) == LOW) {
    for (int i = 9; i <= 11; i++) {
      digitalWrite(i, HIGH);
    }
  }
  if (digitalRead(RIGHT) == LOW) {
    for (int i = 9; i <= 11; i++) {
      digitalWrite(i, LOW);
    }
  }
}

void sound() {  // change tone if needed
  length = pgm_read_word_near(song + 2 * note + 1);
  if ((millis() - timer) >= length) {
    if (note == sizeof(song) / 4 - 1) {
      note = 0;
      octave = octave < POSUN ? octave + 1 : 1;
    } else {
      note++;
    }
    fr = pgm_read_word_near(song + 2 * note);
    timer = millis();
    if (fr != 0) {
      noTone(BEEP);
      delay(length >> 4);
      tone(BEEP, fr * octave);
    } else {
      noTone(BEEP);
    }
  }
}
