
/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

#include <forcedClimate.h>

// Create an instance of a climate sensor. The parameters I2C bus and I2C address are optional. For example:
// - ForcedClimate climateSensor = ForcedClimate(Wire, 0x76);
// - ForcedClimate climateSensor = ForcedClimate(TinyWireM, 0x77);
ForcedClimate climateSensor = ForcedClimate();

void setup(){
	Serial.begin(9600);
	climateSensor.begin();
}

void loop(){
	climateSensor.takeForcedMeasurement();
	// Instead of "takeForcedMeasurement();" one can also use one of getTemperature(true), 
	// getHumidity(true) or getPressure(true) to perform a forced measurement.
	
	Serial.print("Temperature: ");
	Serial.print(climateSensor.getTemperatureCelcius());
	Serial.print(", Humidity: ");
	Serial.print(climateSensor.getRelativeHumidity());
	Serial.print(" and Pressure: ");
	Serial.print(climateSensor.getPressure());
	Serial.println();
	delay(1000);
	Serial.print("Only temperature: ");
	// Perform measurement integrated getTemperature(); useful for when only one value has to be used.
	Serial.print(climateSensor.getTemperatureCelcius(true));
	Serial.println();
	delay(5000);
}
