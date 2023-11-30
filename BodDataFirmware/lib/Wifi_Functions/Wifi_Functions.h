#include <Arduino.h>

#ifndef _WIFI_FUNCTIONS_H
#define _WIFI_FUNCTIONS_H


void WifiBegin(); //Inicia o Wifi com os configurações desejadas
void configPortal(); //Inicia a configuração do Portal
char* getMacAdress(); //Obtém o endereço MAC do aparelho utilizado
void wifiConnection();

#endif