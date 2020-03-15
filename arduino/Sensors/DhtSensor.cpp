#include "Sensors.h"

DHT _dht(DHT_PIN, DHT_TYPE);

DhtSensor::DhtSensor()
{
    _result.temperature = 0;
    _result.humidity = 0;
    _result.heatIndex = 0;
}

void DhtSensor::start()
{
    _dht.begin();
}

void DhtSensor::update()
{
    _result.temperature = _dht.readTemperature();
    _result.humidity = _dht.readHumidity();
    _result.heatIndex = _dht.computeHeatIndex(_result.temperature, _result.humidity, false);
}

DhtResult DhtSensor::read()
{
    return _result;
}
