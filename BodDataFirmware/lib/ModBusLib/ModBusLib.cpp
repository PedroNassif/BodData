#include <Arduino.h>
#include <ModbusMaster.h>
#include "ModBusLib.h"
#include "Draws.h"


//Function Prototypes
bool getMsgError(ModbusMaster *node, uint8_t result);

//globals
static uint32_t prevMillis = 0;
const uint16_t dataTxtTimeInterval = 1000;

void ModbusClass::modbusBegin(ModbusMaster *node){
    RS485Serial.begin(9600, SERIAL_8N2, RX_PIN, TX_PIN);  // Inicializa a comunicação RS485 com a taxa de transmissão desejada
    node->begin(SLAVE_ADD, RS485Serial); // Inicializa o ModbusMaster com o endereço do escravo e a porta Serial RS485
    displayBegin();
    drawIniciando();
}

float ModbusClass::getTemperature(ModbusMaster *node){
    if (millis() - prevMillis >= dataTxtTimeInterval)
    { 
        //iguala o tempo
        prevMillis = millis();  
    
        //Serial.println("Slave " + (String)SLAVE_ADD); //Aqui vai printar o Salve utilizado
        uint8_t result = node->readInputRegisters(0x03E8, 1); //uint8_t readInputRegisters(uint16_t address (Registrador Hexadecimal encontrado na tabela), uint16_t quantity of registers);
  
        if (result == node->ku8MBSuccess){ //Se o resultado da leitura for bem sucedido
            float temp = node->getResponseBuffer(0)/10.0f; 
            drawTemp(temp);
            return temp;
        }
    else
    {
      getMsgError(node, result);
      drawErro();
      return 0.0f;
    }
  } 
  return 0.0f;
}

bool getMsgError(ModbusMaster *node, uint8_t result) {
    String tmpstr2 = "\r\n";
    switch (result) {
        case node->ku8MBIllegalFunction:
        tmpstr2 += "Illegal Function";
        break;
        case node->ku8MBIllegalDataAddress:
        tmpstr2 += "Illegal Data Address";
        break;
        case node->ku8MBIllegalDataValue:
        tmpstr2 += "Illegal Data Value";
        break;
        case node-> ku8MBSlaveDeviceFailure:
        tmpstr2 += "Slave Device Failure";
        break;
        case node->ku8MBInvalidSlaveID:
        tmpstr2 += "Invalid Slave ID";
        break;
        case node->ku8MBInvalidFunction:
        tmpstr2 += "Invalid Function";
        break;
        case node->ku8MBResponseTimedOut:
        tmpstr2 += "Response Timed Out";
        break;
        case node->ku8MBInvalidCRC:
        tmpstr2 += "Invalid CRC";
        break;
        default:
        tmpstr2 += "Unknown error: " + String(result);
        break;
    }
    Serial.println(tmpstr2);
    return false;
}

