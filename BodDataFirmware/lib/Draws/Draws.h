#include <Arduino.h>

#ifndef _DRAWS_H
#define _DRAWS_H

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

void displayBegin();
void drawWifiSymbol();
void drawTemp(float temp);
void drawErro();
void drawIniciando();
void drawWifi();
#endif