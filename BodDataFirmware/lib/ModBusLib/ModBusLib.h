///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MODBUS_H
#define _MODBUS_H

#include <Arduino.h>
#include <ModbusMaster.h>

//Definições=====================================================
#define RS485Serial  Serial2  // Utilize a porta Serial2 para comunicação RS485 (ou outra porta disponível)
#define SLAVE_ADD    1        // Defina o endereço do escravo aqui (pode ser qualquer valor entre 1 e 247) - Definido no proprio Controlador
#define RX_PIN      16        //RX2 
#define TX_PIN      17        //TX2

class ModbusClass{
public:
    //Funções estáticas - sem necessidade de construir objetos
    static void modbusBegin(ModbusMaster *node);
    static float getTemperature(ModbusMaster *node);
    static int autoDetectSlaves(ModbusMaster *node)
    
};

#endif //_MODBUS_H