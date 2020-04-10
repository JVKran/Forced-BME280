
/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

// #define FORCED_CLIMATE_WIRE			// Uncomment this macro in 'Documents/Arduino/Libraries/Forced-BME280/src/forcedClimate.h' to use Wire.h instead of TinyWireM.h
#include <forcedClimate.h>

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
