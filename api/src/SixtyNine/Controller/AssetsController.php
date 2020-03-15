<?php

namespace SixtyNine\Controller;

class AssetsController
{
    private $baseDir = __DIR__.'/../../../assets/';

    public function index($request, $response, $service, $app)
    {
        $service->validateParam('file', 'Required file name')->notNull();
        $file = $request->paramsGet()->get('file');
        $filename = $this->baseDir.$file;
        
        if (!file_exists($filename)) {
            throw new \InvalidArgumentException("File not found");
        }

        return file_get_contents($filename);
    }
}
