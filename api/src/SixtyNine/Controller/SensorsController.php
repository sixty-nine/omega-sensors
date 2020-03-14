<?php

namespace SixtyNine\Controller;

use SixtyNine\Helper\Arduino;

class SensorsController
{
    private $arduino;

    public function __construct(Arduino $arduino)
    {
        $this->arduino = $arduino;
    }

    public function index($request, $response, $service)
    {
        $service->validateParam('id', 'Invalid ID')->isInt();
        $id = $request->paramsGet()->get('id');

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
            default: throw new RuntimeException("Invalid ID");
        }
    }
}
