
/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

#include <forcedClimate.hpp>

// Pass a Wire object for I2C communication and the address of the BME280.
ForcedClimate climateSensor = ForcedClimate(Wire, 0x76);

void setup(){
	Serial.begin(9600);
}

void loop(){
	climateSensor.takeForcedMeasurement();
	// Instead of "takeForcedMeasurement();" one can also use one of getTemperature(true), 
	// getHumidity(true) or getPressure(true) to perform a forced measurement.
	
	Serial.print("Temperature: ");
	Serial.print(climateSensor.getTemperature());
	Serial.print(", Humidity: ");
	Serial.print(climateSensor.getHumidity());
	Serial.print(" and Pressure: ");
	Serial.print(climateSensor.getPressure());
	Serial.println();
	delay(1000);
	Serial.print("Only temperature: ");
	// Perform measurement integrated getTemperature(); useful for when only one value has to be used.
	Serial.print(climateSensor.getTemperature(true));
	Serial.println();
	delay(5000);
}
