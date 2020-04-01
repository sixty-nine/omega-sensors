#ifndef ClientHelper_h
#define ClientHelper_h

#include <Arduino.h>
#include <WiFiEsp.h>

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

#endif
