#include "constants.h"
#include "Timer.h"
#include "Sensors/Sensors.h"

LightSensor light = LightSensor();
DhtSensor dht = DhtSensor();
DallasSensor dallas = DallasSensor();

LightResult lightRes;
DhtResult dhtRes;
DallasResult dallasRes;

void updateSensors()
{
    light.update();
    lightRes = light.read();
    dht.update();
    dhtRes = dht.read();
    dallas.update();
    dallasRes = dallas.read();
}

void outputLight()
{
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
    Serial.print(dhtRes.temperature);
    Serial.print("/");
    Serial.print(dhtRes.humidity);
    Serial.print("/");
    Serial.print(dhtRes.heatIndex);
}

void outputDallas()
{
    Serial.print(dallasRes.temperature);
    Serial.print("/");
    Serial.print(dallasRes.resolution);
    Serial.print("/");
    Serial.print(dallasRes.parasitePower);
}

Timer t = Timer(UPDATE_DELAY, updateSensors);
static const int bufferSize = 30;

void serialEvent()
{
    char buffer[bufferSize];
    int sizeRead = Serial.readBytesUntil('\n', buffer, 1);

    if (bufferSize > 0) {
        switch (buffer[0]) {
            case '1': return outputLight();
            case '2': return outputDHT();
            case '3': return outputDallas();
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
    light.start();
    dht.start();
    dallas.start();
}

void loop()
{
    t.update();
}
