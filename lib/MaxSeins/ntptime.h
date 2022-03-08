





#ifndef _NTPTIME_H
#define _NTPTIM_H

#include <Arduino.h>
#include <string.h>
#include <WiFiUdp.h>
//#include <Interaction.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include "cmddefinitions.h"
#include "globalStorage.cpp"

typedef unsigned long ulong;
typedef uint8_t u8;

class NTPtime   // : public Interaction
{
        public:
            NTPtime(GlobalStorage *gs=nullptr);
            void begin(void);
            unsigned long giveTime(void);
            void makeRequest(void);
            time_t NTP_client(void);
            bool getIsWinter(void);
            time_t checkWinter(time_t t);
            time_t handleManualTime(void);
            
            


        private:
            WiFiUDP udp;
            IPAddress timeServerIP = {178,189,127,149};
            void sendNTPpacket(IPAddress& address);
            bool ntp_runflag = true;
            const int NTP_PACKET_SIZE = 48;
            byte packetBuffer[ 48 ];
            const ulong seventyYears = 2208988800UL;
            ulong highWord;
            ulong lowWord;
            ulong secsSince1900 = 0;
            ulong epoch = seventyYears;
            u8 dayno = 1; // Tagesnumemrn. 
            time_t t = seventyYears;
            uint8_t hoffset = 2;
            //String daynames[7] = {"Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag","Sonntag"};
            uint8_t DST = 6; // Save to EEPROM -> DST 2 nur für Initialisierung. Wenn die Zeit genau über die Umstellung läft, dann ist 1 oder 2.
            void saveDST(void);
            void loadDST(void);
            GlobalStorage *store;

        protected:




};


#endif