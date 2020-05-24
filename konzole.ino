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
#define HEAD 3

// reset
void (*resetFunc)(void) = 0;

byte dino[] = {0x06, 0x0D, 0x0F, 0x0E, 0x1F, 0x1E, 0x0A, 0x0A};
byte tree[] = {0x04, 0x1F, 0x0E, 0x04, 0x1F, 0x0E, 0x04, 0x04};
byte clear[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte head[] = {0x00, 0x0E, 0x15, 0x1F, 0x0E, 0x00, 0x0E, 0x00};

LiquidCrystal_I2C lcd(0x27, 16, 2);

int note = 0;
unsigned long int timerSong;
unsigned long int timerGame;
unsigned long int timerDino;
uint16_t fr;
uint16_t length;
byte octave = 0;
bool dinoGround = true;
uint16_t trees = 0;
byte difficulty = 0x7;
uint16_t refresh = 500;
uint16_t score = 0;

void setup() {
  // LED setup
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, HIGH);
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
  lcd.createChar(HEAD, head);
  lcd.backlight();
  // Serial setup
  Serial.begin(9600);
  // start play music
  fr = pgm_read_word_near(song);
  timerSong = millis();
  tone(BEEP, fr);
  // game init
  timerGame = millis();
  timerDino = millis();
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
  if (left == LOW && dinoGround && millis() - timerDino >= refresh - 100) {
    dinoGround = false;
    timerDino = millis();
  }
  if (!dinoGround && millis() - timerDino >= refresh + 500) {
    dinoGround = true;
    timerDino = millis();
  }
  if (millis() - timerGame >= refresh) {
    if (dinoGround && (trees & 0x4000)) {
      // end
      lcd.setCursor(1, 0);
      lcd.write(CLEAR);
      lcd.setCursor(1, 1);
      lcd.write(HEAD);
      noTone(BEEP);
      while (digitalRead(LEFT))
        ;
      resetFunc();
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
  if (random(0, 2) && (trees & difficulty) == 0) {
    trees <<= 1;
    trees |= 0x1;
  } else {
    trees <<= 1;
  }
  if (trees & 0x8000) {
    score++;
    if (score % 10 == 0) {
      refresh -= 150;
      led();
      difficulty >>= 1;
    }
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

void led() {
  static byte l = 9;
  digitalWrite(l, LOW);
  l++;
  digitalWrite(l, HIGH);
}