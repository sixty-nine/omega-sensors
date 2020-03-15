<?php

require_once(__DIR__.'/../vendor/autoload.php');

define('APP_PATH', '/index.php');

use Doctrine\DBAL\Configuration;
use Doctrine\DBAL\DriverManager;
use Dotenv\Dotenv;
use Dotenv\Exception\ValidationException;
use Klein\Klein;
use Klein\Request;
use SixtyNine\Helper\Arduino;
use SixtyNine\Controller\SensorsController;

const DB_URL = '';

$dotenv = Dotenv::create(__DIR__.'/../');
$dotenv->load();

try {

    $dotenv->required('DATABASE_URL')->notEmpty();

} catch (ValidationException $ex) {
    die('You miss values in your .env file: '.$ex->getMessage());
}

$arduino = new Arduino('/dev/ttyS1');
$controller = new SensorsController($arduino);

$klein = new Klein();
$request = Request::createFromGlobals();

// Remove "/index.php" from the URL
$uri = $request->server()->get('REQUEST_URI');
$request->server()->set('REQUEST_URI', substr($uri, strlen(APP_PATH)));

$klein->respond(static function ($request, $response, $service, $app, $klein) {

    $klein->onError(static function ($klein, $msg) {
        http_response_code(404);
        header('Content-Type: application/json');
        echo json_encode($msg);
        exit();
    });

    $app->register('db', function() {
        static $connection = null;
        if (!$connection) {
            $config = new Configuration();
            $connection = DriverManager::getConnection(['url' => getenv('DATABASE_URL')], $config);
        }
        return $connection;
    });
});

$klein->respond('GET', '/', [$controller, 'index']);
$klein->respond('GET', '/sensors/[:id]', [$controller, 'sensors']);

$klein->respond(function($request, $response, $service, $app, $klein, $matched) {
    if (!$matched->count()) {
        throw new RuntimeException('Not found');
    }
});

$klein->dispatch($request);
