#include <Constants.h>
#include <Data.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(DISPLAY_ADDR, 20U, 4U);

void lcdInit()
{
  lcd.init();
  lcd.clear();
  lcd.setCursor(0U, 0U);
  lcd.backlight();
}

void lcdSetCursor(const uint8_t col, const uint8_t row)
{
  lcd.setCursor(col, row);
}

void lcdPrint(const String &s)
{
  lcd.print(s);
}

void lcdClear()
{
  lcd.clear();
}

void lcdUpdateData()
{
  lcd.setCursor(0U, 0U);
  lcd.print(String(dispTemp, 2) + "C " + String(dispHum) + "% " + String(dispPres) + "hPa");
  lcd.setCursor(0U, 1U);
  lcd.print(String(photoValue) + "Lux");
}
