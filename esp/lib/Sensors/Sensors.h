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
        LightSensor(int lightPin, int resistor = 10000);
        void start();
        void update();
        LightResult read();

    private:
        int _pin;
        int _resistor;
        LightResult _result;
};

class DhtSensor
{
    public:
        DhtSensor(uint8_t pin, uint8_t type);
        void start();
        void update();
        DhtResult read();

    private:
        uint8_t _pin;
        uint8_t _type;
        DHT *_dht;
        DhtResult _result;
};

class DallasSensor
{
    public:
        DallasSensor(uint8_t oneWireBus, uint8_t precision = 9);
        void start();
        void update();
        DallasResult read();

    private:
        uint8_t _precision;
        DallasResult _result;
        OneWire _oneWire;
        DallasTemperature _dallas;
        DeviceAddress _thermometer;
};

#endif
