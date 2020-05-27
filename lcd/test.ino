#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.home();
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readString();
    uint16_t i = s.toInt();
    lcd.write(i);
    lcd.home();
  }
}
