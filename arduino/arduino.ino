#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Timer.h"

#define DHT_PIN 2
#define DHT_TYPE DHT22

#define ONE_WIRE_BUS 3
#define DALLAS_PRECISION 9

#define LIGHT_PIN A1
#define LIGHT_RESISTOR 10000

DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallas(&oneWire);
DeviceAddress thermometer;

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
} DHTResult;

typedef struct {
    const uint8_t *addr;
    float temperature;
    bool parasitePower;
    int8_t resolution;
} DallasResult;

void readLightSensor(LightResult *result)
{
    result->reading = analogRead(LIGHT_PIN);

    // convert the reading to voltage level (0 to 5V)
    result->voltage = result->reading * 5.0 / 1024.0;

    // calculate the resistance of the photoresistor based on voltage level and the voltage divider formula:
    // V(A0) = R1/(R1 + Rphoto) * 5V
    result->resistance = (5.0 / result->voltage * LIGHT_RESISTOR - LIGHT_RESISTOR) / 1000;

    // convert photoresistance resistance to light intensity in lux
    result->intensity = 500 / (result->resistance);
}

void readDHT(DHTResult *result)
{
    result->temperature = dht.readTemperature();
    result->humidity = dht.readHumidity();
    result->heatIndex = dht.computeHeatIndex(result->temperature, result->humidity, false);
}

void readDallas(DallasResult *result)
{
    dallas.requestTemperatures();

    dallas.getAddress(thermometer, 0);
    result->temperature = dallas.getTempC(thermometer);
    result->parasitePower = dallas.isParasitePowerMode();
    result->resolution = dallas.getResolution(thermometer);
    result->addr = thermometer;
}

LightResult lightRes;
DHTResult dhtRes;
DallasResult dallasRes;

void updateSensors()
{
    readLightSensor(&lightRes);
    readDHT(&dhtRes);
    readDallas(&dallasRes);
}

void outputLight()
{
//     Serial.println("--- Light ---------------------");
//     Serial.println(lightRes.reading);
//     Serial.print(lightRes.voltage); Serial.println(" volts");
//     Serial.print(lightRes.resistance); Serial.println(" kohms");
//     Serial.print(lightRes.intensity); Serial.println(" lux");

    Serial.print(lightRes.reading);
    Serial.print("/");
    Serial.print(lightRes.voltage);
    Serial.print("/");
    Serial.print(lightRes.resistance);
    Serial.print("/");
    Serial.print(lightRes.intensity);
}

void outputDHT()
{
//     Serial.println("--- DHT -----------------------");
//     Serial.print(dhtRes.temperature); Serial.println("°C");
//     Serial.print(dhtRes.humidity); Serial.println("%");
//     Serial.print(dhtRes.heatIndex); Serial.println("°C");

    Serial.print(dhtRes.temperature);
    Serial.print("/");
    Serial.print(dhtRes.humidity);
    Serial.print("/");
    Serial.print(dhtRes.heatIndex);
}

void outputDallas()
{
//     Serial.println("--- Dallas --------------------");
//     Serial.print(dallasRes.temperature); Serial.println("°C");
//     Serial.print(dallasRes.resolution); Serial.println("");
//     Serial.print("Parasite power: "); Serial.print(dallasRes.parasitePower ? "On" : "Off"); Serial.println("");

    Serial.print(dallasRes.temperature);
    Serial.print("/");
    Serial.print(dallasRes.resolution);
    Serial.print("/");
    Serial.print(dallasRes.parasitePower);
}

Timer t = Timer(2000, updateSensors);
static const int bufferSize = 30;

void serialEvent()
{
    char buffer[bufferSize];
    int sizeRead = Serial.readBytesUntil('\n', buffer, 1);

    if (bufferSize > 0) {
        switch (buffer[0]) {
            case '1':
                outputLight();
                break;
            case '2':
                outputDHT();
                break;
            case '3':
                outputDallas();
                break;
        }
    }
//     Serial.print("Received: ");
//     Serial.print(bufferSize);
//     Serial.print(" bytes: / ");
//     for (int i = 0 ; i < sizeRead ; i++) {
//         Serial.print(buffer[i], HEX);
//         Serial.print(" / ");
//     }
//     Serial.println();
}

void setup()
{
    Serial.begin(9600);
    pinMode(LIGHT_PIN, INPUT);
    dht.begin();
    dallas.begin();
    if (!dallas.getAddress(thermometer, 0)) {
        // Failed to find the Dallas thermometer
    }
    dallas.setResolution(thermometer, DALLAS_PRECISION);

}

void loop()
{
    t.update();
}
