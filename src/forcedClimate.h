
/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

#ifndef __FORCED_CLIMATE_HPP
#define __FORCED_CLIMATE_HPP

#define FORCED_CLIMATE_WIRE // Comment this macro to use TinyWireM.h instead of Wire.h 

#include <Arduino.h>

#ifdef FORCED_CLIMATE_WIRE
#include <Wire.h> 
#else
#include <TinyWireM.h>
#endif 

class ForcedClimate {
	private:
		#ifndef FORCED_CLIMATE_WIRE
		USI_TWI & bus;
		#else
		TwoWire & bus;	
		#endif
		uint8_t address;

		int16_t temperature[4];
		int16_t pressure[10];
		int16_t humidity[7];
		int32_t BME280t_fine;

		int16_t readTwoRegisters();
		int32_t readFourRegisters();

		void applyOversamplingControls();
		void readCalibrationData();

		enum class registers {
			CTRL_HUM = 0xF2,
			CTRL_MEAS = 0xF4,
			FIRST_CALIB = 0x88,
			SCND_CALIB = 0xE1,
			TEMP_MSB = 0xFA,
			HUM_MSB = 0xFD,
			PRESS_MSB = 0xF7,
			STATUS = 0xF3
		};

	public:
		#ifndef FORCED_CLIMATE_WIRE
		ForcedClimate(USI_TWI & bus, const uint8_t address = 0x76, const bool autoBegin = false);
		#else
		ForcedClimate(TwoWire & bus, const uint8_t address = 0x76, const bool autoBegin = false);	
		#endif

		void begin();
		void takeForcedMeasurement();

		int32_t getIntTemperatureCelcius(const bool performMeasurement = false);
		int32_t getIntTemperatureFahrenheit(const bool performMeasurement = false);
		float getTemperatureCelcius(const bool performMeasurement = false);
		float getTemperatureFahrenheit(const bool performMeasurement = false);

		int32_t getIntPressure(const bool performMeasurement = false);
		float getPressure(const bool performMeasurement = false);

		int32_t getIntRelativeHumidity(const bool performMeasurement = false);
		float getRelativeHumidity(const bool performMeasurement = false);

};


#endif //__FORCED_CLIMATE_HPP
