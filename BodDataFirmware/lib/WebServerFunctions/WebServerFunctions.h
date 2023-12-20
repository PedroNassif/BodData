#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#ifndef _WEB_SERVER_
#define _WEB_SERVER_

class WebServerClass {
public:
    static void notFound(AsyncWebServerRequest *request);
    static void notCreated(AsyncWebServerRequest *request);
    static void handleReset(AsyncWebServerRequest *request);
    static void handleStart(AsyncWebServerRequest *request);
    static void handleDelete(AsyncWebServerRequest *request);
    static void handleDownload(AsyncWebServerRequest *request);
    static void handleHTML(AsyncWebServerRequest *request);

};

#endif //_WEB_SERVER_