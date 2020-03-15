<?php

namespace SixtyNine\Command;

use SixtyNine\Helper\Arduino;
use SixtyNine\Helper\Arrays;
use SixtyNine\Helper\Thingspeak;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

class UpdateThingspeakCommand extends Command
{
    protected function configure()
    {
        $this
            ->setName('update-thingspeak')
            ->setDescription('Update Thingspeak.')
        ;
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $arduino = new Arduino('/dev/ttyS1');
        $arrays = new Arrays();

        $lightsData = $arduino->getLight();
        $dhtData = $arduino->getDht();
        $dallasData = $arduino->getDallas();

        $ts = new Thingspeak(getenv('THINGSPEAK_API_KEY'));
        $ts->sendData(
            $arrays->getValueDotted($lightsData, 'data.intensity'),
            $arrays->getValueDotted($dhtData, 'data.temperature'),
            $arrays->getValueDotted($dhtData, 'data.humidity'),
            $arrays->getValueDotted($dallasData, 'data.temperature')
        );

        $output->writeln('Ok');
    }

}