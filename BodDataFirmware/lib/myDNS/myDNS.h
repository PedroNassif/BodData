#include <Arduino.h>
#include <DNSServer.h>

#ifndef _MY_DNS
#define _MY_DNS

class myDNSClass{
    public:
      //functions
      static void DNSbegin(DNSServer *dnsNode, const IPAddress &serverIP);
};

#endif //_MY_DNS