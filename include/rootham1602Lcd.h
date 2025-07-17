#ifndef LCD_H
#define LCD_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

void initLCD();
void displayMessage(const char *message);
void clearLCD();

#endif