// #include <Arduino.h>
// #include <RTClib.h>
// #include "RtcMyLib.h"

// void RTCClass::rtcBegin(RTC_PCF8563 *node){
//     if (!node->begin()) {
//     Serial.println("Couldn't find RTC");
//     while (1);
//   }

//     if (node->lostPower()) {
//     Serial.println("RTC lost power, let's set the time!");
//     node->adjust(DateTime(F(__DATE__), F(__TIME__)));
//   }
// }

// DateTime::rtcGetTime(RTC_PCF8563 *node){
//     node->now();
// }

// String::rtcPrintTime(RTC_PCF8563 *node){
//   Serial.print(now.year(), DEC);
//   Serial.print('/');
//   Serial.print(now.month(), DEC);
//   Serial.print('/');
//   Serial.print(now.day(), DEC);
//   Serial.print(" ");
//   Serial.print(now.hour(), DEC);
//   Serial.print(':');
//   Serial.print(now.minute(), DEC);
//   Serial.print(':');
//   Serial.print(now.second(), DEC);
//   Serial.println();
// }

// RtcMyLib.cpp

#include "RtcMyLib.h"

void RTCClass::rtcBegin(RTC_PCF8563 *node){
    if (!node->begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (node->lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        node->adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
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

void 
