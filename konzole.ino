#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RED 11
#define YELLOW 10
#define GREEN 9

#define LEFT 7
#define RIGHT 6

#define BEEP 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
    tone(BEEP, 884);
    //lcd setup
    lcd.init();
    lcd.backlight();
    lcd.home();
    lcd.print(F("Omen Charlie"));

    Serial.begin(9600);
    Serial.println(F("test"));
}

void loop() {
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
    delay(1000);
}