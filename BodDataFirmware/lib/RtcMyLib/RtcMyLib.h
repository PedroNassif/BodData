///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RTCLIB
#define _RTCLIB

#include <Arduino.h>
#include <RTClib.h>


class RTCClass{
public:
    //Funções estáticas - sem necessidade de construir objetos
    static void rtcBegin(RTC_PCF8563 *node);
    static DateTime rtcGetTime(RTC_PCF8563 *node);
    static String rtcPrintTime(RTC_PCF8563 *node);

};

#endif //_RTCLIB