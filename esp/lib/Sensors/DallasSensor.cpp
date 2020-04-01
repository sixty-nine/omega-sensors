#include "Sensors.h"

DallasSensor::DallasSensor(uint8_t oneWireBus, uint8_t precision)
{
    _precision = precision;
    _oneWire = OneWire(oneWireBus);
    _dallas = DallasTemperature(&_oneWire);
}

void DallasSensor::start()
{
    _dallas.begin();
    if (!_dallas.getAddress(_thermometer, 0)) {
        // Failed to find the Dallas _thermometer
    }
    _dallas.setResolution(_thermometer, _precision);
}

void DallasSensor::update()
{
    _dallas.requestTemperatures();

    _dallas.getAddress(_thermometer, 0);
    _result.temperature = _dallas.getTempC(_thermometer);
    _result.parasitePower = _dallas.isParasitePowerMode();
    _result.resolution = _dallas.getResolution(_thermometer);
    _result.addr = _thermometer;
}

DallasResult DallasSensor::read()
{
    return _result;
}
