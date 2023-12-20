#include <Arduino.h>

#ifndef _WIFI_FUNCTIONS_H
#define _WIFI_FUNCTIONS_H

#include <WiFiManager.h>

class WifiClass{
    public:
        // //Variaveis
        // static const char*ssid = "Dicalab";                     //Nome(SSID) do WifiLocal
        // static const char* password = "dicalab2763";            //Senha do WifiLocal
        // const char* nome_da_rede = "Esp32-Ap";                  //Define o nome da rede do ConfigPortal

       //Funções
       static void wifiConnection();                            //Conecta ao Wifi
       static void configPortal(WiFiManager *wifiNode);         //Inicia a configuração do Portal
       static char* getMacAdress();                             //Obtém o endereço MAC do aparelho utilizado 
       static void wifiAP();
};

#endif