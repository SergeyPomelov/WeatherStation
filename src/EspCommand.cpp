#include <Arduino.h>
#include <Constants.h>
#include <SoftwareSerial.h>

SoftwareSerial esp8266 = SoftwareSerial(ESP_TX, ESP_RX);

void espBeginSerial()
{
  esp8266.begin(9600U);
}

bool espCmd(const String command, const String expected, const uint64_t timeout, const bool debug)
{
  esp8266.print(command);
  if (debug)
  {
    Serial.print(">>> " + command);
  }

  String response = "";
  uint64_t time = millis();
  bool done = false;

  while (!done && ((time + timeout) > millis()))
  {
    while (!done && esp8266.available())
    {
      char c = esp8266.read();
      response += c;

      if (response.indexOf(expected) > 0)
      {
        done = true;
        if (debug)
        {
          response += " success";
        }
      }
    }
  }

  if (debug)
  {
    response.replace("\r\n", "; ");
    Serial.println("<<< " + response + "  t: " + String((int)(millis() - time)));
  }

  return done;
}

bool espCmd(const String command, const String expected, const uint64_t timeout)
{
  return espCmd(command, expected, timeout, DEBUG);
}

bool espCmd(const String command)
{
  return espCmd(command, "OK", 1000U, DEBUG);
}