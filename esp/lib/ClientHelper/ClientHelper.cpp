#include "ClientHelper.h"

ClientHelper::ClientHelper(WiFiEspClient client)
{
    _client = client;
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
    startResponse(F("text/html"));
}

void ClientHelper::endResponse()
{
    _client.println();
    _client.println();
}

void ClientHelper::print(String msg)
{
    _client.print(msg);
}

void ClientHelper::println(String msg)
{
    _client.print(msg);
    _client.print(F("<br>\n"));
}
