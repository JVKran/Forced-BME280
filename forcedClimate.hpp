
/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

#ifndef __FORCED_CLIMATE_HPP
#define __FORCED_CLIMATE_HPP

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

class ForcedClimate {
	private:
		TwoWire & bus;
		uint8_t address;

		int16_t temperature[4], pressure[10], humidity[7];
		int32_t BME280t_fine;

		enum class registers {
			CTRL_HUM = 0xF2,
			CTRL_MEAS = 0xF4,
			FIRST_CALIB = 0x88,
			SCND_CALIB = 0xE1,
			TEMP_MSB = 0xFA,
			HUM_MSB = 0xFD,
			PRESS_MSB = 0xF7
		};

		int16_t read16 ();
		int32_t read32 ();

		void applyOversamplingControls();
		void readCalibrationData();

	public:
		ForcedClimate(TwoWire & bus, const uint8_t address = 0x76);

		int32_t getTemperature();
		int32_t getPressure();
		int32_t getHumidity();

};


#endif //__FORCED_CLIMATE_HPP
