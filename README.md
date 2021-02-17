# Forced-BME280
There isn't a large amount of small, efficient and easy to use libraries for the BME280; that's what I found out when I needed one.
That's the main goal of this entire library.

The name of this respository stands for the mode in which the BME280 is used. In forced mode the chip sleeps and gets waked up if data is requested. During measurements it peeks up to normal current consumption. However, when not in use, the chip only uses 0.25uA! Great for battery powered ATtiny's ;).

## Advantages of this library
- Ultra low power. In sleep mode the BME280 only uses 0.25uA!
- Ultra small footprint; this library was made with efficiency and memory usage in mind.
- Easy to use interface and functions. Despite the small size of this library, functions are still very intuitive.
- Automatically change between using Wire and TinyWire.


## Example
There has been given an example that can be obtained from below or from within the Arduino IDE in Examples->Forced-BME280->Test.
```c++
#include <forcedClimate.h>

// Create an instance of a BME280. The parameters I2C bus and I2C address are optional. For example:
// - ForcedClimate climateSensor = ForcedClimate(Wire, 0x76);
// - ForcedClimate climateSensor = ForcedClimate(TinyWireM, 0x77);
ForcedClimate climateSensor = ForcedClimate();

void setup(){
	Serial.begin(9600);
	Wire.begin(); 				// Or "TinyWireM.begin()" when using an ATtiny.
	climateSensor.begin();
}

void loop(){
	climateSensor.takeForcedMeasurement();
	Serial.print("Temperature: ");
	Serial.print(climateSensor.getTemperatureCelcius());
	Serial.print(", Humidity: ");
	Serial.print(climateSensor.getRelativeHumidity());
	Serial.print(" and Pressure: ");
	Serial.print(climateSensor.getPressure());
	Serial.println();
	delay(1000);
	// Perform measurement integrated getTemperature(); useful for when only one value has to be used.
	Serial.print(climateSensor.getTemperatureCelcius(true));
	Serial.println();
	delay(5000);
}
```

> This library automatically determines wether to use TinyWireM or Wire. The only thing the user has to take care of, is to make sure begin() is called
on the appropriate instance in the _setup()_; _TinyWireM.begin()_ or _Wire.begin_.

## Functions
#### takeForcedMeasurement() 
This function takes a forced measurement which means getTemperatureCelcius(), getRelativeHumidity() and getPressure() use the updated values. Useful in case all functions are all called at the same time.
#### getTemperatureCelcius(const bool performMeasurement) 
This function can be used to retrieve the temperature. The parameter defaults to false which means takeForcedMeasurement() should be called first to make sure updated values are used. If the passed parameter is equal to true, a forced measurement is taken; useful in case only the getTemperatureCelcius() function is called.
#### getRelativeHumidity(const bool performMeasurement) 
This function can be used to retrieve the humidity. The parameter defaults to false which means takeForcedMeasurement() should be called first to make sure updated values are used. If the passed parameter is equal to true, a forced measurement is taken; useful in case only the getRelativeHumidity() function is called.
#### getPressure(const bool performMeasurement) 
This function can be used to retrieve the pressure. The parameter defaults to false which means takeForcedMeasurement() should be called first to make sure updated values are used. If the passed parameter is equal to true, a forced measurement is taken; useful in case only the getPressure() function is called.


## Installation
#### Download
Press the green _clone or download_ button in the upper-right and download as _.ZIP_. Then go to the Arduino IDE and go to _Sketch_>Use Library->Add .ZIP Library_ and select the just downloaded zip file.

#### Library Manager
Open up the Library Manager in the Arduino IDE and search for *Forced-BME280*. Select the desired version; higher means more features ;).
