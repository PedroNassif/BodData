//Bibliotecas Publicas
#include <Arduino.h>
#include <ModbusMaster.h>
#include <RTClib.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

//Minhas Bibliotecas
#include "Wifi_Functions.h"
#include "WebServerFunctions.h"
#include "ModBusLib.h"
#include "FS_Functions.h"
#include "RtcMyLib.h"
#include "myDNS.h"

//Objetos
AsyncWebServer server(80);
WiFiManager wifi;
ModbusMaster modbus;
RTC_PCF8563 rtc;
DNSServer dnsServer;


//Protótipos de Funções
void initialize();

//Variaveis globais
const char* wifiName= "ESP32_AP";
bool state = false;
IPAddress apIP(192,168, 1, 100);                             // Definindo o IP do Soft AP7
uint8_t slaveNumbers = 0;
uint32_t counter = 0;
float ratios[30][3];


void setup() {
    Serial.begin(115200, SERIAL_8N1);                          //Inicia a Serial
      // Inicialize a EEPROM com o tamanho desejado
    EEPROM.begin(512);
    delay(500);
    uint8_t now = EEPROM.read(1);
    Serial.println();
    Serial.print("O valor de start é: ");
    Serial.println(now);

    if(EEPROM.read(1) != 1){
      EEPROM.put(1, 1);
      delay(400);
      EEPROM.commit();
      uint8_t now2 = EEPROM.read(1);
      EEPROM.put(200, 0);
      EEPROM.commit();
    }

    initialize();
}

void loop(){
  dnsServer.processNextRequest();
  while (state == true ){
    RTCClass::rtcBegin(&rtc);
    DateTime timeRtc = RTCClass::rtcGetTime(&rtc);
    String timestamp = RTCClass::rtcPrintTime(&rtc);
    timestamp += ", ";
    String dataLogger;
  
    float** temperatureRead = ModbusClass::getTemperature(&modbus, slaveNumbers, &rtc);
    if (temperatureRead != nullptr) {
      //Impressão dos dados - datalogger
      FileClass::appendFileString(LittleFS, "/mydir/datalogger.csv", timestamp);
      for(uint8_t i = MIN_NUM_SLAVES; i <= slaveNumbers; i++) {
          for (uint8_t j = 0; j <= 2; j++) {
            dataLogger += String(temperatureRead[i - MIN_NUM_SLAVES][j]);
            ratios[i-1][j] += temperatureRead[i-1][j];
            // Serial.println(ratios[i-1][j]);
            if (i == slaveNumbers && j == 2) {
              } else {
              dataLogger += ", ";
              }
          }
        }
        dataLogger += "\r\n";
        FileClass::appendFileString(LittleFS, "/mydir/datalogger.csv", dataLogger);
      }      
    delay(100);
    counter++;
  }
}

void initialize(){
  if(EEPROM.read(200) == 0){
    //WifiClass::wifiConnection();                             //Inicia a conexão ao Wifi
    WifiClass::wifiAP(apIP, wifiName);                                   //Inicia o SoftAP
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
      WebServerClass::handleRatio(request, counter, ratios, slaveNumbers);
    });

    //Função de Reset do Arquivo
    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
      WebServerClass::handleReset(request, slaveNumbers);
    });

    //Função de delete do Arquivo
    server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
      WebServerClass::handleDelete(request);
      state = false;
    });

    //Função de calibration
      server.on("/calibration", HTTP_GET, [](AsyncWebServerRequest *request){
      WebServerClass::handleCalibration(request);
      RTCClass::rtcCalibration();
    });

    //Função exit do server
    server.on("/exit", HTTP_GET, [](AsyncWebServerRequest *request){
      WebServerClass::handleExit(request);
    });

    //state = true;                                   // Declaração do estado
    server.onNotFound(WebServerClass::notFound);     // Em caso de erro e de não encontrar a página
    server.begin();                                  //Começa o server
  }
  else{
    const char* wifiRtc = "ESP32_CALIBRATION";
    WifiClass::configPortal(&wifi, wifiRtc);
    delay(2000);

    RTCClass::rtcBegin(&rtc);

    // Define o servidor NTP para obter a hora
    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = -10800;
    const int   daylightOffset_sec = 0;

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.println(&timeinfo, "%A, %d %Y %H:%M:%S");
    // Ajuste do RTC com o tempo obtido do servidor NTP
    rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
    RTCClass::rtcPrintTime(&rtc);
    
  
    EEPROM.put(0, 0);
    EEPROM.commit();
    Serial.flush();
    ESP.restart();
  }
}