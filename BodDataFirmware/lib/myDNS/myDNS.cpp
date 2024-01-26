#include <Arduino.h>
#include "myDNS.h"

const byte DNS_PORT = 53;
const char *hostname = "esp32-local";    // Defina o nome de host desejado

void myDNSClass::DNSbegin(DNSServer *dnsNode, const IPAddress &serverIP){
    dnsNode->start(DNS_PORT,"*" , serverIP);
}