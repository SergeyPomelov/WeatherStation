#include <Arduino.h>
#include <Constants.h>
#include <LED.h>
#include <LCD.h>
#include <BME280.h>
#include <Esp.h>
#include <Hibernate.h>

void update()
{
  led(0U, 0U, 50U);
  readSensors();
  // sendData();
  lcdUpdateData();
  led(0U, 0U, 0U);
}

void setup()
{

  ledInit();
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  espBegin();
  Serial.begin(9600U);
  while (!Serial);
  lcdInit();

  bool status = true;
  lcdSetCursor(0U, 0U);
  lcdPrint(F("WiFi... "));
  if (connectWiFi())
  {
    lcdPrint(F("OK"));
  }
  else
  {
    lcdPrint(F("ERROR"));
    status = false;
  }

  lcdSetCursor(0U, 1U);
  lcdPrint(F("BME280... "));
  Serial.print(F("BME280... "));
  if (initBME280())
  {
    lcdPrint(F("OK"));
    Serial.println(F("OK"));
  }
  else
  {
    lcdPrint(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }

  if (status)
  {
    lcdClear();
    update();
  }
  else
  {
    lcdSetCursor(0U, 2U);
    lcdPrint(F("Self-check failed"));
    Serial.println(F("Self-check failed"));
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  hibernate();
  update();
}
