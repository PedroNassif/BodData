#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#ifndef _WEB_SERVER_
#define _WEB_SERVER_

class WebServerClass {
public:
    static void notFound(AsyncWebServerRequest *request);
    static void notCreated(AsyncWebServerRequest *request);
    static void handleReset(AsyncWebServerRequest *request,  uint8_t number);
    static void handleStart(AsyncWebServerRequest *request, bool stt, uint8_t number);
    static void handleDelete(AsyncWebServerRequest *request);
    static void handleDownload(AsyncWebServerRequest *request);
    static void handleHTML(AsyncWebServerRequest *request);
    static void handleRatio(AsyncWebServerRequest *request, uint32_t count, float matrix[30][3], uint8_t number);
    static void handleExit(AsyncWebServerRequest *request);

};

#endif //_WEB_SERVER_