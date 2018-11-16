
#include <arduino.h>
#include "definition.h"
#include "thermistor.h"


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
}





const float   gain = 0.00488;
const float   ref  = 1.25313;

int LeggeTempFumi(){
	unsigned int val = analogRead(TERMOC_PIN);                   // read the input pin
	
	int temperature = uint16_t((float(val) * gain - ref) / 0.005f); // convert to Celsius degree

    return temperature;
}

int LeggeTempAmbiente(){
	int temp = int(thermistor.read() / 10);   // Read temperature

    return temp;
}