#include "WebServerFunctions.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "FS_Functions.h"
#include "minihtml.h"


//Funções do WebServer
void WebServerClass::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Pagina nao encontrada!");
}

void WebServerClass::notCreated(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Arquivo não foi criado, crie ele com https:/ip/start");
}

void WebServerClass::handleReset(AsyncWebServerRequest *request) {
  // Redefinir o conteúdo do arquivo
  FileClass::writeFile(LittleFS, "/mydir/datalogger.csv", "Data , Hora, Temperatura\r\n");
  // Enviar resposta indicando sucesso
  request->send(200, "text/html", htmlCodeReset);
}

void WebServerClass::handleStart(AsyncWebServerRequest *request, bool stt) {
  if(stt == false){
      //Criando arquivo
      FileClass::createDir(LittleFS, "/mydir");
      FileClass::writeFile(LittleFS, "/mydir/datalogger.csv", "Data , Hora, Temperatura\r\n");
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