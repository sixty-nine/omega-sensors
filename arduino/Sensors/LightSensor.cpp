#include "Sensors.h"
#include <Arduino.h>

LightSensor::LightSensor()
{
    _result.reading = 0;
    _result.voltage = 0;
    _result.resistance = 0;
    _result.intensity = 0;
}

void LightSensor::start() {}

void LightSensor::update()
{
    _result.reading = analogRead(LIGHT_PIN);

    // convert the reading to voltage level (0 to 5V)
    _result.voltage = _result.reading * 5.0 / 1024.0;

    // calculate the resistance of the photoresistor based on voltage level and the voltage divider formula:
    // V(A0) = R1/(R1 + Rphoto) * 5V
    _result.resistance = (5.0 / _result.voltage * LIGHT_RESISTOR - LIGHT_RESISTOR) / 1000;

    // convert photoresistance resistance to light intensity in lux
    _result.intensity = 500 / (_result.resistance);

}

LightResult LightSensor::read()
{
    return _result;
}
