





#ifndef _INTERUDP_H
#define _INTERUDP_H

#include <Arduino.h>
#include <string.h>
#include <WiFiUdp.h>
#include "Interaction.h"
#include "cmddefinitions.h"
//#include "ntptime.h"
#include "globalStorage.cpp"
//#include "buttons.h"


class InterUDP //: public Interaction
{


    public:
        InterUDP(GlobalStorage *gs = nullptr);
        
        ~InterUDP(void);
        void beginUDP(int port);
        void runUDP(void);
        //void begin_X(NTPtime *n);
        
        //void checkButtons(void)=0;
        
        Interaction *inter;
        //ButtonsX *buts;

    private:
        int port = UDP_CONTROL_PORT;
        char packetBuffer[56 + 1]; //buffer to hold incoming packet,
        //char  ReplyBuffer[50] = "<ESP OK>>\r\n";       // a string to send back
        void udpprint(String text);
        bool schredInput(char *input);
        String split(char *da, int index);
        WiFiUDP udpa;
        //uint8_t maxgroups = MAX_GROUPS;
        //uint8_t maxscenes = MAX_SCENES;
        //bool printScene(uint8_t scene);
        //bool printGroup(uint8_t scene, uint8_t group);
        String msg_grouperror = String(UDP_ERROR)+";"+String(UDP_ERROR_MSG_GROUP);
        String msg_sceneerror = String(UDP_ERROR)+";"+String(UDP_ERROR_MSG_SCENE);
        String msg_paramerror = String(UDP_ERROR)+";"+String(UDP_ERROR_MSG_PARAM);
        String msg_Tparamerror = String(TIME_ERROR);
        String msg_Pparamerror = String(PARAM_ERROR);
        String msg_undefinederror = String(UDP_ERROR)+";"+String(UDP_ERROR_MSG_UNDEFINED);
        String msg_okerror = String(UDP_ERROR)+";"+String(UDP_ERROR_MSG_OK);
        //NTPtime *ntp;
        GlobalStorage *store;
        


        


};
#endif