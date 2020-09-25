#include <Constants.h>
#include <SoftwareSerial.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

void readSensors();

void initDisplay();

bool initBME280();

void updateDisplay();

void led(const uint8_t red, const uint8_t green, const uint8_t blue);

void hibernate();

bool resetWiFi();

bool connectWiFi();

bool sendData();


extern SoftwareSerial esp8266;
extern LiquidCrystal_I2C lcd;
extern Adafruit_BME280 bme;

extern float dispTemp;
extern uint16_t dispHum;
extern uint16_t dispPres;
extern uint16_t photoValue;