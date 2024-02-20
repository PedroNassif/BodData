///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MODBUS_H
#define _MODBUS_H

#include <Arduino.h>
#include <ModbusMaster.h>

//Definições=====================================================
#define RS485Serial  Serial2     // Utilize a porta Serial2 para comunicação RS485 (ou outra porta disponível)
#define RX_PIN         16        // RX2 
#define TX_PIN         17        // TX2
#define MAX_NUM_SLAVES 30       // Número máximo de escravos do sistemas
#define MIN_NUM_SLAVES 1
// uint32_t AMOSTRAS = 100000;

class ModbusClass{
    public:
        //Variáveis
        static const uint16_t _SAMPLES = 1500;
        static uint32_t prevMillis;
        static const uint16_t dataTxtTimeInterval = 1000;
        static const uint32_t timeToCatch = 10;                                // Valor em segundos
        static const uint8_t MAX_SLAVE_ADDRESS = MAX_NUM_SLAVES;               // Valor máximo para um endereço de escravo Modbus
        static const uint8_t MIN_SLAVE_ADDRESS = MIN_NUM_SLAVES;               // Valor mínimo para um endereço de escravo Modbus
        static uint8_t numSlaves;

        //Funções estáticas - sem necessidade de construir objetos
         static void modbusBegin(ModbusMaster *node);
         static float** getTemperature(ModbusMaster *node, uint8_t slaveNum, RTC_PCF8563 *clock);
         static uint8_t autoDetectSlaves(ModbusMaster *node);
  
    };

#endif //_MODBUS_H