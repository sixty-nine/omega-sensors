#include <Arduino.h>
#include <ArduinoSTL.h>
#include <WiFiEsp.h>

#include <secrets.h>
#include <constants.h>
#include <Timer.h>
#include <Sensors.h>
#include <HardwareInfo.h>
#include <LED.h>
#include <templates.h>

#define FSTRING(s) String(F(s)).c_str()

void sendHttpResponse(WiFiEspClient client);
void sendOkResponse(String msg);
void printWifiStatus();
void updateSensors();
void updateThingSpeak();

class Fan
{
    public:
       static const int SPEED_LOW = 255;
       static const int SPEED_MID1 = 170;
       static const int SPEED_MID2 = 85;
       static const int SPEED_HIGH = 0;

        Fan(byte relayPin, byte controlPin1, byte controlPin2)
        {
            _relayPin = relayPin;
            _controlPin1 = controlPin1;
            _controlPin2 = controlPin2;
        }

        void start(byte speed = Fan::SPEED_HIGH)
        {
            pinMode(_relayPin, OUTPUT);
            pinMode(_controlPin1, OUTPUT);
            pinMode(_controlPin2, OUTPUT);
            analogWrite(_controlPin2, 255);

            setSpeed(speed);
            off();
        }

        void off()
        {
          digitalWrite(_relayPin, LOW);
        }

        void on()
        {
          digitalWrite(_relayPin, HIGH);
        }

        void setSpeed(byte speed)
        {
          analogWrite(_controlPin1, speed);
        }

    private:
        byte _relayPin;
        byte _controlPin1;
        byte _controlPin2;
};

// ------------------

class ClientHelper
{
    public:
        ClientHelper(WiFiEspClient client);
        void startResponse(String contentType);
        void startResponse();
        void endResponse();
        void print(String msg);
        void println(String msg);
    private:
        WiFiEspClient _client;
};

void ClientHelper::startResponse(String contentType)
{
    this->print(F("HTTP/1.1 200 OK\n"));
    this->print(F("Content-type:"));
    this->print(contentType);
    _client.println("\n");
}

void ClientHelper::startResponse()
{
    startResponse("text/html");
}

void ClientHelper::endResponse()
{
    _client.println();
    _client.println();
}

ClientHelper::ClientHelper(WiFiEspClient client)
{
    _client = client;
};

void ClientHelper::print(String msg)
{
    _client.print(msg);
}

void ClientHelper::println(String msg)
{
    _client.print(msg);
    _client.print(F("<br>\n"));
}

// ------------------

int status = WL_IDLE_STATUS;

int ledStatus = LOW;

// Initialize the Ethernet client object
WiFiEspClient client;
WiFiEspServer server(80);

RingBuffer buf(8);

LightSensor light = LightSensor(LIGHT_PIN);
DhtSensor dht = DhtSensor(DHT_PIN, DHT_TYPE);
Timer sensorsTimer = Timer(UPDATE_SENSORS_DELAY, updateSensors);
Timer outputTimer = Timer(OUTPUT_DELAY, updateThingSpeak);
Fan fan = Fan(RELAY_PIN, FAN_PIN_1, FAN_PIN_2);

void setup()
{
    LED led(LED_BUILTIN);
    led.on();

    dht.start();

    Serial.begin(USB_BAUDS);
    Serial1.begin(ESP_BAUDS);
    WiFi.init(&Serial1);

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println(F("WiFi shield not present"));
        // don't continue
        while (true);
    }

    // attempt to connect to WiFi network
    while (status != WL_CONNECTED) {
        Serial.print(F("Attempting to connect to WPA SSID: "));
        Serial.println(SSID);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(SSID, PASSWORD);
    }

    Serial.println(F("You're connected to the network"));
    printWifiStatus();

    // start the web server on port 80
    server.begin();

    // ----

    fan.start();

    led.blink(5, 100);
}


void loop()
{
  sensorsTimer.update();
  outputTimer.update();

  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    Serial.println(F("New client"));             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);

        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        if (buf.endsWith(FSTRING("GET /fan/off"))) {
            fan.off();
            sendOkResponse(F("Fan OFF"));
            break;
        }

        if (buf.endsWith(FSTRING("GET /fan/on"))) {
            fan.on();
            sendOkResponse(F("Fan ON"));
            break;
        }

        if (buf.endsWith(FSTRING("GET /fan/0"))) {
            fan.setSpeed(Fan::SPEED_LOW);
            sendOkResponse(F("Fan speed LOW"));
            break;
        }

        if (buf.endsWith(FSTRING("GET /fan/1"))) {
            fan.setSpeed(Fan::SPEED_MID1);
            sendOkResponse(F("Fan speed MID1"));
            break;
        }

        if (buf.endsWith(FSTRING("GET /fan/2"))) {
            fan.setSpeed(Fan::SPEED_MID2);
            sendOkResponse(F("Fan speed MID2"));
            break;
        }

        if (buf.endsWith(FSTRING("GET /fan/3"))) {
            fan.setSpeed(Fan::SPEED_HIGH);
            sendOkResponse(F("Fan speed HIGH"));
            break;
        }

      }
    }

    // close the connection
    client.stop();
    Serial.println(F("Client disconnected"));
  }
}

void updateSensors()
{
  light.update();
  dht.update();
}

void updateThingSpeak()
{
  LightResult lightRes = light.read();
  DhtResult dhtRes = dht.read();

  const char *url = "/update?api_key=";

  Serial.print("GET ");
  Serial.print(url);
  Serial.print(THINGSPEAK_API_KEY);
  Serial.print("&field1=");
  Serial.print(lightRes.intensity);
  Serial.print("&field2=");
  Serial.print(dhtRes.temperature);
  Serial.print("&field3=");
  Serial.println(dhtRes.humidity);

  if (client.connect("api.thingspeak.com", 80)) {
    Serial.println(F("Connected to server"));
    // Make a HTTP request
    client.print(F("GET "));
    client.print(url);
    client.print(THINGSPEAK_API_KEY);
    client.print("&field1=");
    client.print(lightRes.intensity);
    client.print("&field2=");
    client.print(dhtRes.temperature);
    client.print("&field3=");
    client.print(dhtRes.humidity);
    client.println(" HTTP/1.1");
    client.println(F("Host: api.thingspeak.com"));
    client.println(F("Connection: close"));
    client.println();
    Serial.println(F("Disconnecting from server..."));
    client.stop();
  }
}

void sendOkResponse(String msg)
{
    ClientHelper ch = ClientHelper(client);
    ch.startResponse();
    ch.print(msg);
    ch.endResponse();
}

void sendHttpResponse(WiFiEspClient client)
{
    HardwareInfoRes res = HardwareInfo().getInfo();

    String page = pageTemplate;
    String infoPage = infoPageTemplate;

    ClientHelper ch = ClientHelper(client);
    ch.startResponse();

    infoPage.replace("{{ramFree}}", String(res.ramFree));
    infoPage.replace("{{ramUsed}}", String(res.ramUsed));
    infoPage.replace("{{ramUsedPercent}}", String(res.ramUsedPercent));
    infoPage.replace("{{arduinoVersion}}", String(res.version));
    infoPage.replace("{{appVersion}}", APP_VERSION);

    page.replace("{{content}}", infoPage);

    ch.print(page);
    ch.endResponse();
    return;

//    ClientHelper ch = ClientHelper(client);
//    ch.startResponse();

  // the content of the HTTP response follows the header:
  client.print(F("The LED is "));
  client.print(ledStatus);
  client.println(F("<br>"));
  client.println(F("<br>"));

  client.println(F("LED: <a href=\"/H\">ON</a> <a href=\"/L\">OFF</a><br>"));
  client.println(F("Fan: <a href=\"/9\">ON</a> <a href=\"/0\">OFF</a><br>"));

  client.println(F("Speed:&nbsp;"));
  client.println(F("<a href=\"/1\">1</a>&nbsp;"));
  client.println(F("<a href=\"/2\">2</a>&nbsp;"));
  client.println(F("<a href=\"/3\">3</a>&nbsp;"));
  client.println(F("<a href=\"/4\">4</a><br>"));

//    HardwareInfoRes res = HardwareInfo().getInfo();
//
//    ch.println("RAM Free: " + String(res.ramFree));
//    ch.println("RAM used: " + String(res.ramUsed) + " (" + String(res.ramUsedPercent) + "%)");
//    ch.println("Arduino version: " + String(res.version));

    // ---

    LightResult lightRes = light.read();
    DhtResult dhtRes = dht.read();

    ch.print("Light: " + String(lightRes.intensity) + ", temp: ");
    ch.println(String(dhtRes.temperature) + ", humi: " + String(dhtRes.humidity));

    // ---

    ch.endResponse();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print(F("Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));

  // print where to go in the browser
  Serial.println();
  Serial.print(F("To see this page in action, open a browser to http://"));
  Serial.println(ip);
  Serial.println();
}
