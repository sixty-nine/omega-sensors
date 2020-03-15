<?php

require_once(__DIR__.'/../vendor/autoload.php');

define('APP_PATH', '/index.php');

use Dotenv\Dotenv;
use Klein\Klein;
use Klein\Request;
use SixtyNine\Helper\Arduino;
use SixtyNine\Controller\SensorsController;
use SixtyNine\Controller\AssetsController;

$dotenv = Dotenv::create(__DIR__.'/../');
$dotenv->load();

try {

    $dotenv->required('THINGSPEAK_API_KEY')->notEmpty();

} catch (ValidationException $ex) {
    die('You miss values in your .env file: '.$ex->getMessage());
}

$arduino = new Arduino('/dev/ttyS1');
$sensorsController = new SensorsController($arduino);
$assetsConroller = new AssetsController();

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

$klein->respond('GET', '/', [$sensorsController, 'index']);
$klein->respond('GET', '/sensors/[:id]', [$sensorsController, 'sensors']);
$klein->respond('GET', '/assets', [$assetsConroller, 'index']);

$klein->respond(function($request, $response, $service, $app, $klein, $matched) {
    if (!$matched->count()) {
        throw new RuntimeException('Not found');
    }
});

$klein->dispatch($request);
