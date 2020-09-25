#include <Functions.h>
#include <LowPower.h>
#include <SoftwareSerial.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial esp8266 = SoftwareSerial(ESP_TX, ESP_RX);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(DISPLAY_ADDR, 20U, 4U);
Adafruit_BME280 bme;

float dispTemp;
uint16_t dispHum;
uint16_t dispPres;
uint16_t photoValue;

void led(const uint8_t red, const uint8_t green, const uint8_t blue)
{
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

bool sendDataF(const String command, const String expected, const uint64_t timeout, const bool debug)
{
  esp8266.print(command);
  if (debug)
  {
    Serial.print(">>> " + command);
  }

  String response = "";
  uint64_t time = millis();
  bool ok = false;

  while (!ok && ((time + timeout) > millis()))
  {
    while (!ok && esp8266.available())
    {
      char c = esp8266.read();
      response += c;

      if (response.indexOf(expected) > 0)
      {
        ok = true;
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

  return ok;
}

bool connectWiFi()
{
  led(0U, 50U, 50U);
  sendDataF("AT+CWMODE=1\r\n", "OK", 1000U, DEBUG);
  sendDataF("AT+CIPMUX=1\r\n", "OK", 1000U, DEBUG);
  bool result = sendDataF("AT+CWJAP=\"" + String(SSID) + "\",\"" + PASS + "\"\r\n", "OK", 10000U, DEBUG);
  led(0U, 0U, 0U);
  return result;
}

bool sendData()
{
  sendDataF("AT+CIPSTART=4,\"TCP\",\"" + String(IP) + "\"," + PORT + "\r\n", "OK", 1000U, DEBUG);

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

  sendDataF("AT+CIPSEND=4," + String(cmd.length()) + "\r\n", "OK", 1000U, DEBUG);
  bool result = sendDataF(cmd, "OK", 3000U, DEBUG);
  sendDataF("AT+CIPCLOSE=4\r\n", "CLOSED", 500U, DEBUG);

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

void readSensors()
{
  bme.takeForcedMeasurement();
  dispTemp = bme.readTemperature();
  dispHum = bme.readHumidity();
  dispPres = bme.readPressure() / 100.0F;
  photoValue = analogRead(A0);
  Serial.println(String("Temp ") + dispTemp + " Hum " + dispHum + " Press " + dispPres + " Photo " + photoValue);
}

void initDisplay()
{
  lcd.init();
  lcd.clear();
  lcd.setCursor(0U, 0U);
  lcd.backlight();
}

bool initBME280()
{
  bool result = bme.begin(BME280_ADDRESS, &Wire);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF);
  return result;
}

void updateDisplay()
{
  lcd.setCursor(0U, 0U);
  lcd.print(String(dispTemp, 2) + "C " + String(dispHum) + "% " + String(dispPres) + "hPa");
  lcd.setCursor(0U, 1U);
  lcd.print(String(photoValue) + "Lux");
}

void hibernate()
{
  delay(2000U);
  noInterrupts();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  interrupts();
  delay(1000U);
}

bool resetWiFi()
{
  led(100U, 50U, 0U);
  bool result = sendDataF("AT+RST\r\n", "OK", 1000U, DEBUG) && sendDataF("AT+UART=9600,8,1,0,0\r\n", "OK", 1000U, DEBUG) && sendDataF("AT+CIFSR\r\n", "OK", 1000U, DEBUG);
  led(0U, 0U, 0U);
  return result;
}
