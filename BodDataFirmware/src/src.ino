//Bibliotecas Publicas
#include <Arduino.h>
#include <ModbusMaster.h>
#include <RTClib.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>


//Minhas Bibliotecas
#include "Wifi_Functions.h"
#include "WebServerFunctions.h"
#include "ModBusLib.h"
#include "FS_Functions.h"
#include "RtcMyLib.h"
#include "Draws.h"
#include "myDNS.h"

//Objetos
AsyncWebServer server(80);
WiFiManager wifi;
ModbusMaster modbus;
RTC_PCF8563 rtc;
DNSServer dnsServer;

//Variaveis globais
bool state = false;
const long interval = 60000;                // Intervalo desejado em milissegundos (60 segundos)
IPAddress apIP(192,168, 1, 100);            // Definindo o IP do Soft AP7


void setup() {
  Serial.begin(115200, SERIAL_8N1);         //Inicia a Serial
  WifiClass::wifiAP(apIP);
  //WifiClass::configPortal(&wifi);
  ModbusClass::modbusBegin(&modbus);        //Inicia o Protocolo Modbus
  FileClass::LittleFSbegin();               //Inicia o LittlesFS
  RTCClass::rtcBegin(&rtc);                 //Inicia o RTC
  myDNSClass::DNSbegin(&dnsServer, apIP);     //Inicia o DNS - Domain Name System


  //Função de mostar a pagina HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleHTML(request);
  });

  //Função de start do Arquivo
  server.on("/start", HTTP_GET, [&state](AsyncWebServerRequest *request){
    WebServerClass::handleStart(request, state);
    state = true; 
  });

  //Função de Donwload do Arquivo
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleDownload(request);
  });

  //Função de Reset do Arquivo
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleReset(request);
  });

  //Função de delete do Arquivo
  server.on("/delete", HTTP_GET, [&state](AsyncWebServerRequest *request){
    WebServerClass::handleDelete(request);
    state = false;
  });

  //Função exit do server
  server.on("/exit", HTTP_GET, [](AsyncWebServerRequest *request){
    captivePortalActive = false;
    delay(20000);
    captivePortalActive = true;
  });

  server.onNotFound(WebServerClass::notFound);     // Em caso de erro e de não encontrar a página
  server.begin();                                  //Começa o server
  drawSoftAp();
}

void loop(){
  dnsServer.processNextRequest();
  
  while (state == true )
  {
      DateTime timeRtc = RTCClass::rtcGetTime(&rtc);
      String timestamp = RTCClass::rtcPrintTime(&rtc);
      float temperatura = ModbusClass::getTemperature(&modbus);    //"Temperatura" recebe o valor lido
      Serial.println(temperatura);
      //String stringTemperatura = String(temperatura);
      //String stringTime = String(timestamp);
      String dataLogger = String(timestamp) + ", " + String(temperatura) + "\r\n";
      const char* dataToAppend = dataLogger.c_str();
      FileClass::appendFile(LittleFS, "/mydir/datalogger.csv", dataToAppend);
      
      //Caso queira que os logs apareçam no Monitor descomente essa linha
      //FileClass::readFile(LittleFS, "/mydir/datalogger.csv");
      
      delay(interval);
  }
}
