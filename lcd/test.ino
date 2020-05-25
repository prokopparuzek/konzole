#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.cursor();
  lcd.home();
  lcd.print("AHOJ");
  lcd.leftToRight();
  lcd.setCursor(0, 1);
  for (int i = 0; i < 10; i++) {
    lcd.print(i);
    delay(500);
  }
  lcd.autoscroll();
}

void loop() {}
