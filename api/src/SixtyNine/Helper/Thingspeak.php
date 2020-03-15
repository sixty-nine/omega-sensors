<?php

namespace SixtyNine\Helper;

use GuzzleHttp\Client;

class Thingspeak
{
    /** @var string */
    private $apiKey;
    /** @var Client */
    private $client;

    public function __construct($apiKey)
    {
        $this->apiKey = $apiKey;
        $this->client = new Client();
    }

    public function sendData($light, $inTemp, $inHumi, $outTemp)
    {
        return $this->client->request('GET', 'https://api.thingspeak.com/update', [
            'form_params' => [
                'api_key' => $this->apiKey,
                'field1' => $light,
                'field2' => $inTemp,
                'field3' => $inHumi,
                'field4' => $outTemp,
            ]
        ]);
    }
}