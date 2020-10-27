/**
 * Test of SHT30 and BMP280. The sketch reads the main values.
 */
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WEMOS_SHT3X.h>

const int sclPin = 1; //9;
const int sdaPin = 2; //8;

Adafruit_BMP280 bmp;
SHT3X sht30(0x45);

void setup() {
  Serial.begin(115200);
  Serial.println("BMP280 and SHT30 test");

  Wire.begin(sdaPin, sclPin);

  if(sht30.get()==0) {
    Serial.println("Found SHT30 sensor");
  } else {
    Serial.println("Could not find a valid SHT30 sensor, check wiring!");
    while(1);
  }

  if (bmp.begin(0x76)){
    Serial.println("Found BMP280 sensor");
  } else {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.println();

    if (sht30.get()==0) {
      Serial.print("Temperature in Celsius : ");
      Serial.println(sht30.cTemp);
      Serial.print("Temperature in Fahrenheit : ");
      Serial.println(sht30.fTemp);
      Serial.print("Relative Humidity : ");
      Serial.println(sht30.humidity);
      Serial.println();
    } else {
      Serial.println("Error!");
    }
    delay(2000);
}
