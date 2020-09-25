#include <Arduino.h>
#include <Functions.h>

void update() {
  led(0U, 0U, 50U);
  readSensors();
  sendData();
  updateDisplay();
  led(0U, 0U, 0U);
}

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  esp8266.begin(9600U);
  Serial.begin(9600U); while (!Serial);
  initDisplay();

  bool status = true;
  lcd.setCursor(0U, 0U);
  lcd.print(F("WiFi... "));
  if (connectWiFi()) {
    lcd.print(F("OK"));
  } else {
    lcd.print(F("ERROR"));
    status = false;
  }

  lcd.setCursor(0U, 1U);
  lcd.print(F("BME280... "));
  Serial.print(F("BME280... "));
  if (initBME280()) {
    lcd.print(F("OK"));
    Serial.println(F("OK"));
  } else {
    lcd.print(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }

  if (status) {
    lcd.clear();
    update();
  } else {
    lcd.print(F("Self-check failed"));
    Serial.println(F("Self-check failed"));
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  hibernate();
  update();
}
