
/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

#ifndef __FORCED_CLIMATE_HPP
#define __FORCED_CLIMATE_HPP

#if defined(__AVR_ATtiny25__) | defined(__AVR_ATtiny45__) | defined(__AVR_ATtiny85__) | defined(__AVR_AT90Tiny26__) | defined(__AVR_ATtiny26__)
	#define FORCED_CLIMATE_ATTINY
#endif

#include <Arduino.h>

#ifdef FORCED_CLIMATE_ATTINY
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif 

class ForcedClimate {
	private:
		#ifdef FORCED_CLIMATE_ATTINY
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
		#ifdef FORCED_CLIMATE_ATTINY
		ForcedClimate(USI_TWI & bus = TinyWireM, const uint8_t address = 0x76, const bool autoBegin = false);
		#else
		ForcedClimate(TwoWire & bus = Wire, const uint8_t address = 0x76, const bool autoBegin = false);	
		#endif

		void begin();
		void takeForcedMeasurement();

		#ifdef FORCED_CLIMATE_ATTINY
		int32_t getTemperatureCelcius(const bool performMeasurement = false);
		#else
		float getTemperatureCelcius(const bool performMeasurement = false);
		#endif

		#ifdef FORCED_CLIMATE_ATTINY
		int32_t getPressure(const bool performMeasurement = false);
		#else
		float getPressure(const bool performMeasurement = false);
		#endif

		#ifdef FORCED_CLIMATE_ATTINY
		int32_t getRelativeHumidity(const bool performMeasurement = false);
		#else
		float getRelativeHumidity(const bool performMeasurement = false);
		#endif

};


#endif //__FORCED_CLIMATE_HPP
