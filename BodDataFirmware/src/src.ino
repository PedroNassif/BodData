#include <Arduino.h>
#include <ModbusMaster.h>
#include "ModBusLib.h"
#include "FS_Functions.h"


//Objetos
ModbusMaster modbus;

void setup() {
 Serial.begin(115200, SERIAL_8N1);       //Inicia a Serial
 ModbusClass::modbusBegin(&modbus);     //Inicia o Protocolo Modbus
 FileClass::LittleFSbegin();
 FileClass::listDir(LittleFS, "/", 3);
 FileClass::createDir(LittleFS, "/mydir");
 FileClass::writeFile(LittleFS, "/mydir/tempdata.csv", "Time , Temp\r\n");
}

void loop(){
  float temperatura = ModbusClass::getTemperature(&modbus);    //"Temperatura" recebe o valor lido
  Serial.println(temperatura);
  String stringTemperatura = String(temperatura);
  String tempToAppend = String("Time, " + stringTemperatura + "\r\n");
  const char* dataToAppend = tempToAppend.c_str();
  FileClass::appendFile(LittleFS, "/mydir/tempdata.csv", dataToAppend);
  FileClass::readFile(LittleFS, "/mydir/tempdata.csv");
  delay(10000);
}
