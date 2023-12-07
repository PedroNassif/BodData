#include <Arduino.h>
#include <ModbusMaster.h>
#include "ModBusLib.h"
#include "FS_Functions.h"
#include <WiFiManager.h>
#include "Wifi_Functions.h"
#include <ESPAsyncWebServer.h>
#include "WebServerFunctions.h"
#include <time.h>


//Objetos
AsyncWebServer server(80);
WiFiManager wifi;
ModbusMaster modbus;

//Variaveis globais
bool state = false;

void setup() {
  Serial.begin(115200, SERIAL_8N1);       //Inicia a Serial
  WifiClass::wifiConnection();
  ModbusClass::modbusBegin(&modbus);     //Inicia o Protocolo Modbus
  FileClass::LittleFSbegin();            //Inicia o LittlesFS

    // Ajuste o fuso horário conforme necessário
    configTime(-3 * 3600, 0, "pool.ntp.org");

    // Tente obter a hora do servidor NTP
    time_t now;
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
      Serial.println("Falha ao obter a hora. Tentando novamente...");
      delay(1000);
    }

  //Função de Donwload do Arquivo
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleDownload(request);
  });

  //Função de Reset do Arquivo
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    WebServerClass::handleReset(request);
  });

  //Função de start do Arquivo
  server.on("/start", HTTP_GET, [&state](AsyncWebServerRequest *request){
    WebServerClass::handleStart(request);
    state = true;
  });

  //Função de delete do Arquivo
  server.on("/delete", HTTP_GET, [&state](AsyncWebServerRequest *request){
    WebServerClass::handleDelete(request);
    state = false;
  });

  // //Função de mostar o IP na tela
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   WebServerClass::handleIP(request);
  // });

  server.onNotFound(WebServerClass::notFound);     //Caso não encontre um server ativo
  server.begin();                                  //Começa o server
}

void loop(){
  while (state == true )
  {
        //Ajustando o timestamp
        time_t now;
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
          Serial.println("Falha ao obter a hora");
          delay(1000);
          continue;
        }
        now = mktime(&timeinfo);
        // Formatando o timestamp para a string
        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "%d-%m-%y, %H:%M:%S", &timeinfo);
        
      float temperatura = ModbusClass::getTemperature(&modbus);    //"Temperatura" recebe o valor lido
      Serial.println(temperatura);
      //String stringTemperatura = String(temperatura);
      //String stringTime = String(timestamp);
      String dataLogger = String(timestamp) + ", " + String(temperatura) + "\r\n";
      const char* dataToAppend = dataLogger.c_str();
      FileClass::appendFile(LittleFS, "/mydir/datalogger.csv", dataToAppend);
      //FileClass::readFile(LittleFS, "/mydir/datalogger.csv");
      delay(10000);
  }
}
