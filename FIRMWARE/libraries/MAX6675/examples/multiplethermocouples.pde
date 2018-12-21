#include "max6675.h"
#include <SPI.h>

int thermoCS1 = 5;
int thermoCS2 = 6;

MAX6675 thermocouple1;
MAX6675 thermocouple2;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  thermocouple1.begin(thermoCS);
  thermocouple2.begin(thermoCS);

  Serial.println("MAX6675 test");
  // wait for MAX chips to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp

   Serial.print("C = ");
   Serial.println(thermocouple1.readCelsius());
   Serial.print("F = ");
   Serial.println(thermocouple1.readFahrenheit());
   Serial.print("C = ");
   Serial.println(thermocouple2.readCelsius());
   Serial.print("F = ");
   Serial.println(thermocouple2.readFahrenheit());

   delay(1000);
}
