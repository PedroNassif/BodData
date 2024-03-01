#include <Arduino.h>
#include <WiFiManager.h>
#include "Wifi_Functions.h"
#include "Draws.h"

//Variaveis
const char* ssid = "Dicalab";                            //Nome(SSID) do WifiLocal
const char* password = "dicalab2763";                   //Senha do WifiLocal
const char* nome_da_rede = "Esp32-Ap";                  //Define o nome da rede do ConfigPortal

//Flags
bool shouldSaveConfig = false;

//Variáveis globais
byte mac[6];
char mac_Id[18];

void WifiClass::configPortal (WiFiManager *wifiNode, const char* name){
  wifiNode->resetSettings();
  WiFi.mode(WIFI_AP_STA);
  if (!wifiNode->startConfigPortal((char*)name)) {
    Serial.println("Falha ao Conectar ao Portal");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  //if you get here you have connected to the WiFi
  Serial.println("Conectado!");
  // shouldSaveConfig = true;
}

char* WifiClass::getMacAdress(){
    //Lendo o Mac Adrees do Esp32
    WiFi.macAddress(mac);
    snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.print("O Mac Adress é: ");
    Serial.println(mac_Id);
    return mac_Id;
}

void WifiClass::wifiConnection(){
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

void WifiClass::wifiAP(const IPAddress &serverIP, const char* name){
  WiFi.mode(WIFI_AP);
  WiFi.softAP(name);
  WiFi.softAPConfig(serverIP, serverIP, IPAddress(255, 255, 255, 0));
  Serial.print("Endereço IP do AP: ");
  Serial.println(serverIP);
}

//void WifiClass::
