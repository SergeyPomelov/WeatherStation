#include <Constants.h>
#include <Data.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

bool initBME280()
{
  bool result = bme.begin(BME280_ADDRESS_ALTERNATE, &Wire);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF);
  return result;
}

void readSensors()
{
  bme.takeForcedMeasurement();
  dispTemp = bme.readTemperature();
  dispHum = bme.readHumidity();
  dispPres = bme.readPressure();
  photoValue = analogRead(A3);
  coValue = analogRead(A2);
  Serial.println(String("Temp ") + dispTemp + " Hum " + dispHum + " Press " + dispPres + " Photo " + photoValue + " CO " + coValue);
}
