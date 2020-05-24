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

#define DINO 0
#define TREE 1
#define CLEAR 2

#define DINO_POSITION 1

// reset
void (*resetFunc)(void) = 0;

byte dino[] = {0x06, 0x0D, 0x0F, 0x0E, 0x1F, 0x1E, 0x0A, 0x0A};
byte tree[] = {0x04, 0x1F, 0x0E, 0x04, 0x1F, 0x0E, 0x04, 0x04};
byte clear[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

LiquidCrystal_I2C lcd(0x27, 16, 2);

int note = 0;
unsigned long int timerSong;
unsigned long int timerGame;
uint16_t fr;
uint16_t length;
byte octave = 0;
bool dinoUp = false;
bool dinoGround = true;
uint16_t trees = 0;
byte difficulty = 0xE;
uint16_t score = 0;
uint16_t refresh = 500;

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
  // random setup
  randomSeed(analogRead(A0));
  // lcd setup
  lcd.init();
  lcd.createChar(DINO, dino);
  lcd.createChar(TREE, tree);
  lcd.createChar(CLEAR, clear);
  lcd.backlight();
  // Serial setup
  Serial.begin(9600);
  // start play music
  fr = pgm_read_word_near(song);
  timerSong = millis();
  tone(BEEP, fr);
  // game init
  timerGame = millis();
  lcd.clear();
  moveTrees();
  showDino();
  showTrees();
}

void loop() {
  sound();  // play music
  game();
}

void game() {
  bool left = digitalRead(LEFT);
  if (left == LOW && dinoGround) {
    dinoUp = true;
    dinoGround = false;
    showDino();
  }
  if (millis() - timerGame >= refresh) {
    if (dinoGround && (trees & 0x4000)) {
      // end
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print(F("The END"));
      lcd.setCursor(4, 1);
      lcd.print(score);
      noTone(BEEP);
      while (digitalRead(LEFT))
        ;
      resetFunc();
    }
    if (!dinoUp && !dinoGround) {
      dinoGround = true;
    }
    if (dinoUp && !dinoGround) {
      dinoUp = false;
    }
    lcd.clear();
    moveTrees();
    showDino();
    showTrees();
    printScore();
    timerGame = millis();
  }
}

void showDino() {
  if (dinoGround) {
    lcd.setCursor(1, 0);
    lcd.write(CLEAR);
    lcd.setCursor(1, 1);
  } else {
    lcd.setCursor(1, 1);
    lcd.write(CLEAR);
    lcd.setCursor(1, 0);
  }
  lcd.write(DINO);
}

void printScore() {
  lcd.setCursor(6, 0);
  lcd.print(F("Score:"));
  lcd.print(score);
}

void moveTrees() {
  trees <<= 1;
  if (random(0, 2) && (trees & difficulty) == 0) {
    trees |= 0x1;
  }
  if (trees & 0x8000) {
    score++;
  }
}

void showTrees() {
  uint16_t mask = 0x8000;
  for (int i = 0; i < 16; i++) {
    if (trees & mask) {
      lcd.setCursor(i, 1);
      lcd.write(TREE);
    }
    mask >>= 1;
  }
}

void sound() {  // change tone if needed
  length = pgm_read_word_near(song + 2 * note + 1);
  if ((millis() - timerSong) >= length) {
    if (note == sizeof(song) / 4 - 1) {
      note = 0;
      octave = octave < POSUN ? octave + 1 : 0;
    } else {
      note++;
    }
    fr = pgm_read_word_near(song + 2 * note);
    timerSong = millis();
    if (fr != 0) {
      noTone(BEEP);
      delay(length >> 4);
      tone(BEEP, fr << octave);
    } else {
      noTone(BEEP);
    }
  }
}
