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
const long interval = 3000;                                  // Intervalo desejado em milissegundos (60 segundos)
IPAddress apIP(192,168, 1, 100);                             // Definindo o IP do Soft AP7
uint8_t slaveNumbers = 0;
float instaReadMatrix[MAX_NUM_SLAVES][AMOSTRAS];             // Crie um array de arrays para armazenar os valores de cada escravo [i][j]
float ratioReadMatrix[MAX_NUM_SLAVES][3];                    // Crie um array de arrays para armazenar os valores de cada escravo [i][j]




void setup() {
  Serial.begin(115200, SERIAL_8N1);                          //Inicia a Serial
  //WifiClass::wifiConnection();                             //Inicia a conexão ao Wifi
  WifiClass::wifiAP(apIP);                                   //Inicia o SoftAP
  //ModbusClass::modbusBegin(&modbus);                       //Inicia o Protocolo Modbus
  slaveNumbers = ModbusClass::autoDetectSlaves(&modbus);     //Inicia a AutoDetectação dos Slaves no sistema
  FileClass::LittleFSbegin();                                //Inicia o LittlesFS
  RTCClass::rtcBegin(&rtc);                                  //Inicia o RTC
  myDNSClass::DNSbegin(&dnsServer, apIP);                    //Inicia o DNS - Domain Name System


  //Função de mostar a pagina HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleHTML(request);
  });

  //Função de start do Arquivo
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleStart(request, state, slaveNumbers);
    state = true; 
  });

  //Função de Donwload do Arquivo
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleDownload(request);
  });

   //Função de Donwload do Arquivo
  server.on("/ratio", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleRatio(request);
  });

  //Função de Reset do Arquivo
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleReset(request);
  });

  //Função de delete do Arquivo
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleDelete(request);
    state = false;
  });

  // //Função exit do server
  // server.on("/exit", HTTP_GET, [](AsyncWebServerRequest *request){
    
  // });

  // state = false;                                   // Declaração do estado
  server.onNotFound(WebServerClass::notFound);     // Em caso de erro e de não encontrar a página
  server.begin();                                  //Começa o server
  drawSoftAp();
}

void loop(){
  dnsServer.processNextRequest();
  
  while (state == true ) //mudar estado para teste
  {
      DateTime timeRtc = RTCClass::rtcGetTime(&rtc);
      String timestamp = RTCClass::rtcPrintTime(&rtc);
      timestamp += ", ";
      String dataLogger;
    

     float** temperatureRead = ModbusClass::getTemperature(&modbus, slaveNumbers, instaReadMatrix);
     if (temperatureRead != nullptr) {
        for(uint8_t i = MIN_NUM_SLAVES; i <= slaveNumbers; i++) {
          for (uint8_t j = 0; j <= 2; j++) {
            ratioReadMatrix[i -MIN_NUM_SLAVES][j] = temperatureRead[i - MIN_NUM_SLAVES][j];
          }
        }
      }
      
    //Impressão dos dados - datalogger
    FileClass::appendFileString(LittleFS, "/mydir/datalogger.csv", timestamp);
    for(uint8_t i = MIN_NUM_SLAVES; i <= slaveNumbers; i++) {
        for (uint8_t j = 0; j <= 2; j++) {
          dataLogger += String(temperatureRead[i - MIN_NUM_SLAVES][j]);
          if (j < 3 ) {
            dataLogger += ", ";
          }
        }
      }
      dataLogger += "\r\n";
      FileClass::appendFileString(LittleFS, "/mydir/datalogger.csv", dataLogger);

      //Caso queira que os logs apareçam no Monitor descomente essa linha
      //FileClass::readFile(LittleFS, "/mydir/datalogger.csv");
      
      delay(interval);
  }
}

