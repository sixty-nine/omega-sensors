<?php

require_once(__DIR__.'/../vendor/autoload.php');

define('APP_PATH', '/index.php');

use Klein\Klein;
use Klein\Request;
use SixtyNine\Helper\Arduino;
use SixtyNine\Controller\SensorsController;

$arduino = new Arduino('/dev/ttyS1');
$controller = new SensorsController($arduino);

$klein = new Klein();
$request = Request::createFromGlobals();

// Remove "/index.php" from the URL
$uri = $request->server()->get('REQUEST_URI');
$request->server()->set('REQUEST_URI', substr($uri, strlen(APP_PATH)));

$klein->respond(static function () use ($klein) {
    $klein->onError(static function ($klein, $msg) {
        http_response_code(404);
        header('Content-Type: application/json');
        echo json_encode($msg);
        exit();
    });
});

$klein->respond('GET', '/sensors', [$controller, 'index']);

$klein->respond(function($request, $response, $service, $app, $klein, $matched) {
    if (!$matched->count()) {
        throw new RuntimeException('Not found');
    }
});

$klein->dispatch($request);
