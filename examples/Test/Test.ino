#include <forcedClimate.hpp>

ForcedClimate climateSensor = ForcedClimate(Wire, 0x76);		// Pass a Wire object for I2C communication and the address of the BME280.

void setup(){
	Serial.begin(9600);
}

void loop(){
	Serial.print("Temperature: ");
	Serial.print(climateSensor.getTemperature());
	Serial.print(", Humidity: ");
	Serial.print(climateSensor.getHumidity());
	Serial.print(" and Pressure: ");
	Serial.print(climateSensor.getPressure());
	Serial.println();
	delay(10000);
}
