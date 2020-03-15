<?php

namespace SixtyNine\Controller;

use GuzzleHttp\Client;
use SixtyNine\Helper\Arduino;
use SixtyNine\Helper\Arrays;

class SensorsController
{
    private $arduino;

    public function __construct(Arduino $arduino)
    {
        $this->arduino = $arduino;
    }

    public function index($request, $response, $service, $app)
    {
        $lightsData = $this->arduino->getLight();
        $dhtData = $this->arduino->getDht();
        $dallasData = $this->arduino->getDallas();

        $service->arrays = new Arrays();
        $service->render(__DIR__.'/../views/dashboard.phtml', [
            'data' => [
                'light' => $lightsData,
                'dht' => $dhtData,
                'dallas' => $dallasData,
            ]
        ]);
    }

    public function sensors($request, $response, $service, $app)
    {
        $service->validateParam('id', 'Invalid ID')->isInt();
        $id = $request->paramsNamed()->get('id');

        $response->header('Content-Type', 'application/json');

        switch ($id) {
            case '0':return json_encode([
                'light' => $this->arduino->getLight(),
                'dht' => $this->arduino->getDht(),
                'dallas' => $this->arduino->getDallas(),
            ]);
            case '1': return json_encode($this->arduino->getLight());
            case '2': return json_encode($this->arduino->getDht());
            case '3': return json_encode($this->arduino->getDallas());
            default: throw new \RuntimeException('Invalid ID');
        }
    }
}
