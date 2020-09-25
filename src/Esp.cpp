#include <Constants.h>
#include <Data.h>
#include <LED.h>
#include <EspCommand.h>

void espBegin()
{
  espBeginSerial();
}

bool connectWiFi()
{
  led(0U, 50U, 50U);
  espCmd("AT+CWMODE=1\r\n", "OK", 1000U);
  espCmd("AT+CIPMUX=1\r\n", "OK", 1000U);
  bool result = espCmd("AT+CWJAP=\"" + String(SSID) + "\",\"" + PASS + "\"\r\n", "OK", 10000U);
  led(0U, 0U, 0U);
  return result;
}

bool sendData()
{
  espCmd("AT+CIPSTART=4,\"TCP\",\"" + String(IP) + "\"," + PORT + "\r\n");

  String content = String("{\r\n") + "\"deviceId\": \"arduino_weather_station\"" + ",\r\n" + "\"temperature\": " + String(dispTemp) + ",\r\n" + "\"humidity\": " + String(dispHum) + "\r\n}";

  String cmd = "POST /data HTTP/1.1\r\n";
  cmd += String("Host: ") + IP + ":" + PORT + "\r\n";
  cmd += "User-Agent: Arduino/ESP8266\r\n";
  cmd += "Connection: close\r\n";
  cmd += "Content-Type: application/json\r\n";
  cmd += "Content-Length: ";
  cmd += content.length();
  cmd += "\r\n\r\n";
  cmd += content;
  cmd += "\r\n";

  espCmd("AT+CIPSEND=4," + String(cmd.length()) + "\r\n");
  bool result = espCmd(cmd, "OK", 3000U);
  espCmd("AT+CIPCLOSE=4\r\n", "CLOSED", 500U);

  if (!result)
  {
    Serial.print(F("Reconnecting WiFi... "));
    result = connectWiFi();
    if (!result)
    {
      Serial.println(F("OK"));
    }
    else
    {
      Serial.println(F("ERROR"));
    }
  }
  return result;
}

bool resetWiFi()
{
  led(100U, 50U, 0U);
  bool result = espCmd("AT+RST\r\n") && espCmd("AT+UART=9600,8,1,0,0\r\n") && espCmd("AT+CIFSR\r\n");
  led(0U, 0U, 0U);
  return result;
}
