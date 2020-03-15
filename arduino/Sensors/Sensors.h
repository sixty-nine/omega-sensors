#ifndef Sensors_h
#define Sensors_h

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

typedef struct {
    int reading;
    float voltage;      // volts
    float resistance;   // kOhms
    float intensity;    // Lux
} LightResult;

typedef struct {
    float temperature;
    float humidity;
    float heatIndex;
} DhtResult;

typedef struct {
    const uint8_t *addr;
    float temperature;
    bool parasitePower;
    int8_t resolution;
} DallasResult;

class LightSensor
{
    public:
        LightSensor();
        void start();
        void update();
        LightResult read();

    private:
        LightResult _result;
};

class DhtSensor
{
    public:
        DhtSensor();
        void start();
        void update();
        DhtResult read();

    private:
        DhtResult _result;
};

class DallasSensor
{
    public:
        DallasSensor();
        void start();
        void update();
        DallasResult read();

    private:
        DallasResult _result;
        OneWire _oneWire;
        DallasTemperature _dallas;
        DeviceAddress _thermometer;
};

#include "LightSensor.cpp"
#include "DhtSensor.cpp"
#include "DallasSensor.cpp"

#endif
