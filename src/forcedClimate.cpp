/// @file

/* Forced-BME280 Library
   Jochem van Kranenburg - jochemvk.duckdns.org - 9 March 2020
*/

#include "forcedClimate.h"

/// \brief
/// Read 16 bits
/// \details
/// This function reads 16 bits from the I2C bus.
int16_t ForcedClimate::readTwoRegisters () {
    uint8_t lo = bus.read();
    uint8_t hi = bus.read();
    return hi << 8 | lo;
}

/// \brief
/// Read 32 bits
/// \details
/// This function reads 32 bits from the I2C bus.
int32_t ForcedClimate::readFourRegisters () {
    uint8_t msb = bus.read();
    uint8_t lsb = bus.read();
    uint8_t xlsb = bus.read();
    return (uint32_t)msb << 12 | (uint32_t)lsb << 4 | (xlsb >> 4 & 0x0F);
}

/// \brief
/// Constructor
/// \details
/// This creates a ForcedClimate object from the mandatory TwoWire-bus
/// and the address of the chip to communicate with.
#ifndef FORCED_CLIMATE_WIRE
ForcedClimate::ForcedClimate(USI_TWI & bus, const uint8_t address, const bool autoBegin):
#else
ForcedClimate::ForcedClimate(TwoWire & bus, const uint8_t address, const bool autoBegin):   
#endif
    bus(bus),
    address(address)
{
    if(autoBegin){
        bus.begin();
        delay(2);
        applyOversamplingControls();
        readCalibrationData();
    }
}

/// \brief
/// Begin
/// \details
/// This applies the set Oversampling Controls and reads the calibration
/// data from the register. This function has been implementted to comply
/// with the Arduino 1.5 Format; it's also called from the constructor (as it should be).
void ForcedClimate::begin(){
    applyOversamplingControls();
    readCalibrationData();
}

/// \brief
/// Take forced measurement
/// \details
/// This function takes a forced measurement. That is, the BME280 is woken up to take
/// a measurement after which it goes back to sleep. During this sleep, it consumes
/// 0.25uA!
void ForcedClimate::takeForcedMeasurement(){
    bus.beginTransmission(address);
    bus.write((uint8_t)registers::CTRL_MEAS);
    bus.write(0b00100101);
    bus.endTransmission();
}

/// \brief
/// Apply oversampling controls
/// \details
/// This function sets the sampling controls to values that are
/// suitable for all kinds of applications.
void ForcedClimate::applyOversamplingControls(){
    bus.beginTransmission(address);
    bus.write((uint8_t)registers::CTRL_HUM);
    bus.write(0b00000001);
    bus.write((uint8_t)registers::CTRL_MEAS);
    bus.write(0b00100101);                          // Last two bits are 01 for forced, 11 for normal and 00 for sleep mode
    bus.write((uint8_t)registers::FIRST_CALIB);
    bus.endTransmission();
}

/// \brief
/// Read Calibrations
/// \details
/// This functions reads the calibration data after which it is
/// stored in the temperature, pressure and humidity arrays for
/// later use in compensation.
void ForcedClimate::readCalibrationData(){
    bus.requestFrom(address, (uint8_t)26);
    for (int i=1; i<=3; i++) temperature[i] = readTwoRegisters();           // Temperature
    for (int i=1; i<=9; i++) pressure[i] = readTwoRegisters();              // Pressure
    bus.read();                                                             // Skip 0xA0
    humidity[1] = (uint8_t)bus.read();                                      // Humidity
    bus.beginTransmission(address);
    bus.write((uint8_t)registers::SCND_CALIB);
    bus.endTransmission();
    bus.requestFrom(address, (uint8_t)7);
    humidity[2] = readTwoRegisters();
    humidity[3] = (uint8_t)bus.read();
    uint8_t e4 = bus.read(); uint8_t e5 = bus.read();
    humidity[4] = ((int16_t)((e4 << 4) + (e5 & 0x0F)));
    humidity[5] = ((int16_t)((bus.read() << 4) + ((e5 >> 4) & 0x0F)));
    humidity[6] = ((int8_t)bus.read());
    getTemperatureCelcius();
}

/// \brief
/// Get Temperature Celcius
/// \details
/// This function retrieves the compensated temperature as described
/// on page 50 of the BME280 Datasheet.
int32_t ForcedClimate::getIntTemperatureCelcius(const bool performMeasurement){
    bus.beginTransmission(address);
    if(performMeasurement){
        bus.write((uint8_t)registers::CTRL_MEAS);
        bus.write(0b00100101);
    }
    bus.write((uint8_t)registers::TEMP_MSB);
    bus.endTransmission();
    bus.requestFrom(address, (uint8_t)3);
    int32_t adc = readFourRegisters();
    int32_t var1 = ((((adc>>3) - ((int32_t)((uint16_t)temperature[1])<<1))) * ((int32_t)temperature[2])) >> 11;
    int32_t var2 = ((((adc>>4) - ((int32_t)((uint16_t)temperature[1]))) * ((adc>>4) - ((int32_t)((uint16_t)temperature[1])))) >> 12);
    var2 = (var2 * ((int32_t)temperature[3])) >> 14;
    BME280t_fine = var1 + var2;
    return (BME280t_fine*5+128)>>8;
}

/// \brief
/// Get Temperature Celcius
/// \details
/// This function retrieves the compensated temperature as described
/// on page 50 of the BME280 Datasheet.
float ForcedClimate::getTemperatureCelcius(const bool performMeasurement){
    return float(getIntTemperatureCelcius(performMeasurement) / 100);
}

/// \brief
/// Get Temperature Fahrenheit
/// \details
/// This function retrieves the compensated temperature as described
/// on page 50 of the BME280 Datasheet.
int32_t ForcedClimate::getIntTemperatureFahrenheit(const bool performMeasurement){
    return int32_t(float(getTemperatureCelcius() * 1.8) + 32);
}

/// \brief
/// Get Temperature Fahrenheit
/// \details
/// This function retrieves the compensated temperature as described
/// on page 50 of the BME280 Datasheet.
float ForcedClimate::getTemperatureFahrenheit(const bool performMeasurement){
    return float(float(float(getIntTemperatureCelcius() * 1.8) + 32) / 100);
}

/// \brief
/// Get Pressure
/// \details
/// This function retrieves the compensated pressure as described
/// on page 50 of the BME280 Datasheet.
int32_t ForcedClimate::getIntPressure(const bool performMeasurement){
    bus.beginTransmission(address);
    if(performMeasurement){
        bus.write((uint8_t)registers::CTRL_MEAS);
        bus.write(0b00100101);
    }
    bus.write((uint8_t)registers::PRESS_MSB);
    bus.endTransmission();
    bus.requestFrom(address, (uint8_t)3);

    int32_t adc = readFourRegisters();
    int32_t var1 = (((int32_t)BME280t_fine)>>1) - (int32_t)64000;
    int32_t var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)pressure[6]);
    var2 = var2 + ((var1*((int32_t)pressure[5]))<<1);
    var2 = (var2>>2) + (((int32_t)pressure[4])<<16);
    var1 = (((pressure[3] * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)pressure[2]) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((int32_t)((uint16_t)pressure[1])))>>15);

    if (var1 == 0){
        return 0;
    }
    uint32_t p = (((uint32_t)(((int32_t)1048576) - adc) - (var2>>12)))*3125;
    if (p < 0x80000000){
        p = (p << 1) / ((uint32_t)var1);
    } else {
        p = (p / (uint32_t)var1) * 2;
    }

    var1 = (((int32_t)pressure[9]) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)pressure[8]))>>13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + pressure[7]) >> 4));
    return p;
}

/// \brief
/// Get Pressure
/// \details
/// This function retrieves the compensated pressure as described
/// on page 50 of the BME280 Datasheet.
float ForcedClimate::getPressure(const bool performMeasurement){
    return float(getIntPressure(performMeasurement) / 100);
}

/// \brief
/// Get Humidity
/// \details
/// This function retrieves the compensated humidity as described
/// on page 50 of the BME280 Datasheet.
int32_t ForcedClimate::getIntRelativeHumidity(const bool performMeasurement){
    bus.beginTransmission(address);
    if(performMeasurement){
        bus.write((uint8_t)registers::CTRL_MEAS);
        bus.write(0b00100101);
    }
    bus.write((uint8_t)registers::HUM_MSB);
    bus.endTransmission();
    bus.requestFrom(address, (uint8_t)2);
    uint8_t hi = bus.read(); uint8_t lo = bus.read();
    int32_t adc = (uint16_t)(hi<<8 | lo);
    int32_t var1;
    var1 = (BME280t_fine - ((int32_t)76800));
    var1 = (((((adc << 14) - (((int32_t)humidity[4]) << 20) - (((int32_t)humidity[5]) * var1)) +
              ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)humidity[6])) >> 10) * (((var1 *
                                                                                           ((int32_t)humidity[3])) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
                                           ((int32_t)humidity[2]) + 8192) >> 14));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)humidity[1])) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);
    return (uint32_t)((var1>>12)*25)>>8;
}

/// \brief
/// Get Humidity
/// \details
/// This function retrieves the compensated humidity as described
/// on page 50 of the BME280 Datasheet.
float ForcedClimate::getRelativeHumidity(const bool performMeasurement){
    return float(getIntRelativeHumidity(performMeasurement) / 100);
}
