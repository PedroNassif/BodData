#include <Arduino.h>
#include <WiFiManager.h>
#include "Wifi_Functions.h"
#include "Draws.h"

//criação de objetos
WiFiManager wm;

//Variaveis que tomarão conta da conexão Wifi
const char* ssid = "Dicalab";
const char* password = "dicalab2763";

//Nome da rede do config Portal
const char* nome_da_rede = "Esp32-Ap"; //Define o nome da rede

//Flags
bool shouldSaveConfig = false;

//Variáveis globais
byte mac[6];
char mac_Id[18];

void configPortal (){
 
 if (!wm.autoConnect(nome_da_rede)){
   Serial.println("Falha ao Conectar automaticamente");

   if (!wm.startConfigPortal(nome_da_rede)) {
      Serial.println("Falha ao Conectar ao Portal");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("Conectado!");
    shouldSaveConfig = true;
 }
 else{  
  Serial.println("Conectado automáticamente!");
  drawWifi();
  }
}

char* getMacAdress(){
    //Lendo o Mac Adrees do Esp32
    WiFi.macAddress(mac);
    snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.print("O Mac Adress é: ");
    Serial.println(mac_Id);
    return mac_Id;
}

void wifiConnection(){
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
