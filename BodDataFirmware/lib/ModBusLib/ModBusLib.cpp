#include <Arduino.h>
#include <ModbusMaster.h>
#include "ModBusLib.h"
#include "Draws.h"


//Function Prototypes
bool getMsgError(ModbusMaster *node, uint8_t result);

//globals
static uint32_t prevMillis = 0;
const uint16_t dataTxtTimeInterval = 1000;
const uint8_t MAX_SLAVE_ADDRESS = MAX_NUM_SLAVES;               // Valor máximo para um endereço de escravo Modbus
const uint8_t MIN_SLAVE_ADDRESS = MIN_NUM_SLAVES;                // Valor mínimo para um endereço de escravo Modbus
float temp = 0;
uint8_t numSlaves = 0;

void printMatrix(float matrix[][AMOSTRAS], uint8_t numRows, uint8_t numCols) {
    Serial.println("Matrix:");
    for (uint8_t i = 0; i < numRows; i++) {
        Serial.print("Slave ");
        Serial.print(i + MIN_SLAVE_ADDRESS);
        Serial.print(": ");
        for (uint8_t j = 0; j < numCols; j++) {
            Serial.print(matrix[i][j], 2); // Imprime o valor com duas casas decimais
            Serial.print(" ");
        }
        Serial.println();
    }
}

void printRatioMatrix(float** ratioMatrix, uint8_t numRows) {
    Serial.println("Ratio Matrix:");
    for (uint8_t i = 0; i < numRows; i++) {
        Serial.print("Slave ");
        Serial.print(i + MIN_SLAVE_ADDRESS);
        Serial.print(": [");
        for (uint8_t j = 0; j < 3; j++) {
            Serial.print(ratioMatrix[i][j], 2); // Imprime o valor com duas casas decimais
            if (j < 2) {
                Serial.print(", ");
            }
        }
        Serial.println("]");
    }
}

void ModbusClass::modbusBegin(ModbusMaster *node){
    RS485Serial.begin(9600, SERIAL_8N2, RX_PIN, TX_PIN);  // Inicializa a comunicação RS485 com a taxa de transmissão desejada
    for(uint8_t i = MIN_SLAVE_ADDRESS; i <= numSlaves; i++){
    node->begin(i, RS485Serial); // Inicializa o ModbusMaster com o endereço do escravo e a porta Serial RS485
    }
    displayBegin();
    drawIniciando();
}

float** ModbusClass::getTemperature(ModbusMaster *node, uint8_t slaveNum, float matrix[][AMOSTRAS]){
    float** ratioMatrix = new float*[slaveNum];         //Matix que vai guardar astemperaturas [Min, Med, MAx] de todos os escavros

    if (millis() - prevMillis >= dataTxtTimeInterval){ 
        prevMillis = millis();    //iguala o tempo
        for(uint8_t i = MIN_SLAVE_ADDRESS; i <= slaveNum; i++){
            float minTemp = 40;                         // Inicializa com um valor grande
            float maxTemp = 10;                         // Inicializa com um valor pequeno
            float sumTemp = 0;
            float medTemp = 0;

            ratioMatrix[i-1] = new float[3];

            for(uint8_t j = 0; j < AMOSTRAS ; j++){
                
                node ->_u8MBSlave = i;   // Define o endereço do escravo
                uint8_t result = node->readInputRegisters(0x03E8, 1); //uint8_t readInputRegisters(uint16_t address (Registrador Hexadecimal encontrado na tabela), uint16_t quantity of registers);

                if (result == node->ku8MBSuccess){ //Se o resultado da leitura for bem sucedido
                    temp = node->getResponseBuffer(0)/10.0f;
                    matrix[i-1][j] = temp;

                    minTemp = min(minTemp, matrix[i-1][j]);
                    maxTemp = max(maxTemp, matrix[i-1][j]);
                    sumTemp += temp;
                                      
                    ratioMatrix[i-1][0] = minTemp;
                    ratioMatrix[i-1][2] = maxTemp;
                    
                    drawTemp(temp);     
                }
                else{
                    getMsgError(node, result);
                    drawErro();
                    
                    // Libera a memória antes de retornar a função
                    for (uint8_t k = 0; k < slaveNum; k++) {
                        delete[] ratioMatrix[k];
                    }
                    return nullptr ;
                }
                delay(50);
            }
            medTemp = sumTemp/AMOSTRAS;
            ratioMatrix[i-1][1] = medTemp;
            
            
        }
        printMatrix(matrix, slaveNum, AMOSTRAS);
        printRatioMatrix(ratioMatrix, slaveNum);
       
        return ratioMatrix;  
    } 
    return nullptr ;
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

uint8_t ModbusClass::autoDetectSlaves(ModbusMaster *node) {
    uint8_t errorCount = 0;
    RS485Serial.begin(9600, SERIAL_8N2, RX_PIN, TX_PIN);
    for(uint8_t i = MIN_SLAVE_ADDRESS; i <= MAX_SLAVE_ADDRESS; i++){
        if(errorCount <= 4){
            node->begin(i,RS485Serial);
            uint8_t result = node->readInputRegisters(0x03E8,1);
            if(result == node->ku8MBSuccess){
                Serial.print("Escravo encontrado em: ");
                Serial.println(i);
                numSlaves++;
                errorCount = 0;
            }
            else{
                errorCount++;
                Serial.println("Escravo não encontrado");
            }
            delay(50); //Taxa de espera para dar tempo de voltar a respota do primeiro slave
        }
    }  
    displayBegin();
    drawIniciando();
    return numSlaves;
}

// void getRatioTemperature(ModbusMaster *node, uint8_t slaveNum){
//     float* temperatureRead = ModbusClass::getTemperature(node, slaveNum);    //"Temperatura" recebe o valor lido
//     float slaveMatrix[slaveNum][AMOSTRAS];
//     for (uint8_t i = MIN_SLAVE_ADDRESS; i <= slaveNum; i++) {
//         for (uint16_t j = 0; j <= AMOSTRAS; j++){
//             slaveMatrix[i - MIN_SLAVE_ADDRESS][j] = temperatureRead[i];
//         }
//   }
// }