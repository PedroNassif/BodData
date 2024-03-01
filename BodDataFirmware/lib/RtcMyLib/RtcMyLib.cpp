#include "RtcMyLib.h"
#include "Wifi_Functions.h"
#include <WiFiManager.h>
#include <time.h>
#include <EEPROM.h>


//functions prototype
void printLocalTime();

void RTCClass::rtcBegin(RTC_PCF8563 *node){
    if (!node->begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (node->lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        node->adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    node->start();
}

DateTime RTCClass::rtcGetTime(RTC_PCF8563 *node){
    return node->now();
}

String RTCClass::rtcPrintTime(RTC_PCF8563 *node){
    DateTime now = node->now();
    String result = String(now.day()) + '/' + String(now.month()) + '/' + String(now.year()) + ',' + String(now.hour()) + ':' + String(now.minute()) + ':' + String(now.second());
    Serial.println(result);
    return result;
}

void RTCClass::rtcCalibration(){ 
    EEPROM.put(200, 1);
    EEPROM.commit();
    ESP.restart();
}

