
#include <arduino.h>
#include "definition.h"
#include "thermistor.h"
#include "max6675.h"
#include <SPI.h>

#define use_MAX6675

#ifdef use_MAX6675
MAX6675 thermocouple;
#endif // use_MAX6675



THERMISTOR thermistor(NTC_PIN,             // Analog pin
                      _NTC_NOM_RESISTANCE, // Nominal resistance at 25 ºC
                      _NTC_BETA,           // thermistor's beta coefficient
                      _NTC_SERIES_RES);    // Value of the series resistor

// Thermistor object
void AnalogInit(){
    //thermistor.analogPin = _NTC_PIN;
    thermistor.nominalResistance = _NTC_NOM_RESISTANCE;
    thermistor.bCoefficient = _NTC_BETA;
    thermistor.serialResistance = _NTC_SERIES_RES;
#ifdef use_MAX6675
	SPI.begin();
	thermocouple.begin(MAX6675_CS);
	//Serial.println(MAX6675_CS);
#endif // use_MAX6675

}


#ifdef use_MAX6675
int LeggeTempFumi() {
	double t = thermocouple.readCelsius();
	Serial.println(t);
	return (int)t;
}


#else
const float   gain = 0.00488;
const float   ref  = 1.25313;

int LeggeTempFumi(){
	unsigned int val = analogRead(TERMOC_PIN);                   // read the input pin
	
	int temperature = uint16_t((float(val) * gain - ref) / 0.005f); // convert to Celsius degree

    return temperature;
}
#endif // use_MAX6675





int LeggeTempAmbiente(){
	int temp = int(thermistor.read() / 10);   // Read temperature

    return temp;
}