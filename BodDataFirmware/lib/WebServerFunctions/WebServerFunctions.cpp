#include "WebServerFunctions.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "FS_Functions.h"
#include "minihtml.h"

String htmlTempConst = "";


//Funções do WebServer
void WebServerClass::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Pagina nao encontrada!");
}

void WebServerClass::notCreated(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Arquivo não foi criado, crie ele com https:/ip/start");
}

void WebServerClass::handleReset(AsyncWebServerRequest *request, uint8_t number) {
  // Redefinir o conteúdo do arquivo
  FileClass::writeFile(LittleFS, "/mydir/datalogger.csv", "Data , Hora"); //Temperatura Mínima , Temperatura Média, Temperatura Máxima\r\n
  for(uint8_t i = 1; i <= number ; i++){
      FileClass::appendFileString(LittleFS, "/mydir/datalogger.csv", String(", Temperatura Mínima Escravo ") + String(i) + String(", Temperatura Média Escravo ") + String(i) + String(", Temperatura Máxima Escravo ") + String(i));
    }
  FileClass::appendFile(LittleFS, "/mydir/datalogger.csv", "\r\n");

  // Enviar resposta indicando sucesso
  request->send(200, "text/html", htmlCodeReset);
}

void WebServerClass::handleStart(AsyncWebServerRequest *request, bool stt, uint8_t number) {
  if(stt == false){
      //Criando arquivo
      FileClass::createDir(LittleFS, "/mydir");
      FileClass::writeFile(LittleFS, "/mydir/datalogger.csv", "Data , Hora"); //Temperatura Mínima , Temperatura Média, Temperatura Máxima\r\n
      for(uint8_t i = 1; i <= number ; i++){
      FileClass::appendFileString(LittleFS, "/mydir/datalogger.csv", String(", Temperatura Mínima Escravo ") + String(i) + String(", Temperatura Média Escravo ") + String(i) + String(", Temperatura Máxima Escravo ") + String(i));
      }
      FileClass::appendFile(LittleFS, "/mydir/datalogger.csv", "\r\n");

      // Enviar resposta indicando sucesso
      request->send(200, "text/html", htmlCodeStart);
  }else{
  request->send(200, "text/html", htmlCodeAlreadyStart);
  }
}

void WebServerClass::handleDelete(AsyncWebServerRequest *request){
  //Deletando arquivo
 FileClass::deleteFile(LittleFS, "/mydir/datalogger.csv");
  // Enviar resposta indicando sucesso
  request->send(200, "text/html", htmlCodeDelete);
}

void WebServerClass::handleDownload(AsyncWebServerRequest *request){
    // Abrir o arquivo para leitura
    File file = LittleFS.open( "/mydir/datalogger.csv", "r");

    if (!file) {
        request->send(404, "text/plain", "Arquivo nao encontrado");
        return;
    }
    // Enviar o conteúdo do arquivo como resposta
    request->send(LittleFS, "/mydir/datalogger.csv", "text/csv");
    // Fechar o arquivo
    file.close();
}

void WebServerClass::handleHTML(AsyncWebServerRequest *request){
  request->send(200, "text/html", htmlCodeRoot);
}

void WebServerClass::handleRatio(AsyncWebServerRequest *request, uint32_t count, float matrix[30][3], uint8_t number){
  float matrizRatio[number][3];
  String append;
  for(uint8_t i = 0; i < number; i++){
    for(uint8_t j = 0; j < 3; j++){
      matrizRatio[i][j] = (matrix[i][j])/count;
      Serial.println(matrizRatio[i][j]);
      
    }
    append += "{ slave: " + String(i) + ", min: " + String(matrizRatio[i][0]) + ", avg: " + String(matrizRatio[i][1]) + ", max: " + String(matrizRatio[i][2]) + " },";
  }
  
  
  //Criando arquivo
      String htmlRatioPage1 = R"(<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>Central de Controle - DicaLab</title><style>*{font-family:Quicksand,sans-serif}body{text-align:center;margin:20px;background-color:#333;color:#fff}h1{margin-bottom:20px;color:#4f66e7f5;font-size:x-large}.box{background-color:#444;border-radius:10px;padding:10px;margin-bottom:20px;margin-left:10px;margin-right:10px;display:inline-block}.box h2{color:#60e920;font-size:large;margin-bottom:10px}.temperature{color:#fff;font-size:1.2rem}button{border:0;background-color:#1fa3ec;color:#fff;border-radius:5px;width:35%;padding:10px;font-size:1.2rem;cursor:pointer;margin-bottom:15px;transition:background-color .3s}button:active{background-color:#114b74}button:hover{background-color:#155a8a}button.D{background-color:#dc3630;width:20%}button.D:hover{background-color:#9b1915}button.D:active{background-color:#b91d18}</style></head><body><div><h1>Temperaturas Médias dos Equipamentos</h1><hr><br><div id="temperatureBoxes"></div><form action="/" method="get"><button class="D">Voltar à Central de Ações</button></form></div><script>const temperatures = [ )"; 


      String htmlRatioPage2 = R"( ];
        function createTemperatureBoxes() {
        const temperatureBoxes = document.getElementById('temperatureBoxes');

        temperatures.forEach(temperature => {
        const box = document.createElement('div');
        box.classList.add('box');

        const h2 = document.createElement('h2');
        h2.textContent = `Escravo ${temperature.slave}`;
        box.appendChild(h2);

        const p1 = document.createElement('p');
        p1.classList.add('temperature');
        p1.textContent = `Mínima: ${temperature.min}°C`;
        box.appendChild(p1);

        const p2 = document.createElement('p');
        p2.classList.add('temperature');
        p2.textContent = `Média: ${temperature.avg}°C`;
        box.appendChild(p2);

        const p3 = document.createElement('p');
        p3.classList.add('temperature');
        p3.textContent = `Máxima: ${temperature.max}°C`;
        box.appendChild(p3);

        temperatureBoxes.appendChild(box);
        });
      }

        // Chama a função quando a página terminar de carregar
        window.onload = function() {
        createTemperatureBoxes();
        };</script></body></html>)";

  String htmlRatioPage = htmlRatioPage1 + append + htmlRatioPage2;
 
  request->send(200, "text/html", htmlRatioPage);

}

void WebServerClass::handleExit(AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Você saiu do portal, se quiser reconectar digite o IP: 192.168.1.100 na barra de pesqueisa do seu navegador.");
}