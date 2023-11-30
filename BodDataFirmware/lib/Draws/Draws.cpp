#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Draws.h"

Adafruit_SSD1306 display (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayBegin(){
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, true); //config. da tela
}

void drawWifiSymbol() {
  display.drawLine(113,28,127,28,WHITE);
  display.drawLine(115,29,125,29,WHITE);
  display.drawLine(117,30,123,30,WHITE);
  display.drawLine(119,31,121,31,WHITE);
  display.display(); 
}

void drawTemp(float temp){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Temperatura:");
  display.setTextSize(2);
  display.setCursor(10,10);
  display.print(temp);
  display.print(" C");
  display.display(); 
}

void drawErro(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,10);
  display.print("Tem algo errado!");
  display.display(); 
}

void drawIniciando(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Iniciando...");
  display.display(); 
}

void drawWifi(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Conectado ao Wifi!");
  display.display(); 
}