<?php

namespace SixtyNine\Helper;

use PhpSerial\PhpSerial;

class Arduino
{
    /** @var PhpSerial */
    private $serial;

    public function __construct(string $port, $bauds = 9600)
    {
        $this->serial = new PhpSerial;
        $this->serial->deviceSet($port);
        $this->serial->confBaudRate($bauds);
        $this->serial->deviceOpen();
    }

    public function __destruct()
    {
        $this->serial->deviceClose();
    }

    private function sendMessage(string $message): array
    {
        $this->serial->sendMessage($message);
        $read = $this->serial->readPort();
        return explode('/', $read);
    }

    public function getLight(): array
    {
        [$reading, $voltage, $resistor, $intensity] = $this->sendMessage('1');
        return [
            'data' => [
                'reading' => $reading,
                'voltage' => $voltage,
                'resistor' => $resistor,
                'intensity' => $intensity,
            ],
            'metadata' => [
                'sensor' => 'photocell',
                'units' => [
                    'voltage' => 'V',
                    'resistor' => 'kOhm',
                    'intensity' => 'Lux',
                ],
            ],
        ];
    }

    public function getDht(): array
    {
        [$temperature, $humidity, $heatIndex] = $this->sendMessage('2');
        return [
            'data' => [
                'temperature' => $temperature,
                'humidity' => $humidity,
                'heatIndex' => $heatIndex,
            ],
            'metadata' => [
                'sensor' => 'dht',
                'units' => [
                    'voltage' => '°C',
                    'resistor' => '%',
                    'intensity' => '°C',
                ],
            ],
        ];
    }

    public function getDallas(): array
    {
        [$temperature, $resolution, $parasitePower] = $this->sendMessage('3');
        return [
            'data' => [
                'temperature' => $temperature,
                'resolution' => $resolution,
                'parasitePower' => $parasitePower,
            ],
            'metadata' => [
                'sensor' => 'dallas',
                'units' => [
                    'voltage' => '°C',
                ],
            ],
        ];
    }
}
