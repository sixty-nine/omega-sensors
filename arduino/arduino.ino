#include "constants.h"
#include "Timer.h"
#include "Sensors/Sensors.h"

LightSensor light = LightSensor();
DhtSensor dht = DhtSensor();
DallasSensor dallas = DallasSensor();

LightResult lightRes;
DhtResult dhtRes;
DallasResult dallasRes;

void outputLight()
{
    Serial.print(lightRes.reading);
    Serial.print("/");
    Serial.print(lightRes.voltage);
    Serial.print("/");
    Serial.print(lightRes.resistance);
    Serial.print("/");
    Serial.println(lightRes.intensity);
}

void outputDHT()
{
    Serial.print(dhtRes.temperature);
    Serial.print("/");
    Serial.print(dhtRes.humidity);
    Serial.print("/");
    Serial.println(dhtRes.heatIndex);
}

void outputDallas()
{
    Serial.print(dallasRes.temperature);
    Serial.print("/");
    Serial.print(dallasRes.resolution);
    Serial.print("/");
    Serial.println(dallasRes.parasitePower);
}

unsigned long ledInterval = 100;
bool ledBlinking = false;
bool ledOn = false;

Timer t = Timer(UPDATE_DELAY, updateSensors);
Timer t1 = Timer(ledInterval, updateLed);
static const int bufferSize = 30;

void updateLed() {
  static int count = 0;

  if (!ledBlinking) {
    digitalWrite(LED_PIN, HIGH);
    return;
  }

  ledOn = !ledOn;
  digitalWrite(LED_PIN, (ledOn ? HIGH : LOW));
  count++;

  if (count > 5) {
    ledBlinking = false;
    count = 0;
  }
}

void updateSensors()
{
    light.update();
    lightRes = light.read();
    dht.update();
    dhtRes = dht.read();
    dallas.update();
    dallasRes = dallas.read();
}

void serialEvent()
{
    char buffer[bufferSize];
    int sizeRead = Serial.readBytesUntil('\n', buffer, 1);

    if (bufferSize > 0) {

        ledBlinking = true;
        delay(0);

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
            case '0':
              outputLight();
              outputDHT();
              outputDallas();
              break;
        }
    }
}

void setup()
{
    pinMode(LIGHT_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    Serial.begin(9600);

    light.start();
    dht.start();
    dallas.start();
}

void loop()
{
    t.update();
    t1.update();
}
