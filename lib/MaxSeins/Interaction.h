
#ifndef _INTERACTION_H
#define _INTERACTION_H


#include <Arduino.h>
#include <string.h>
#include <MiLightUdpServer.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include "buttons.h"
#include "cmddefinitions.h"
#include "globalStorage.cpp"
//#include "testklasse.h"


typedef uint8_t u8;
typedef unsigned long ulong;

class Interaction
{

// Scene Modes : Single Scene: eine die ganze zeitspanne, multi: mehrere szenen mit times, manual: manuelle szenenauswahl. 


    

    public:
        Interaction(GlobalStorage *gs = nullptr);
        ~Interaction(void);
        

        //uint8_t SCENE_MODE = MODE_AUTO; //MODE_OFF, MODE_AUTO, MODE_MANUAL
        //uint8_t ACTIVE_SCENE = 1; //von 1-6
        //uint8_t SELECTED_SCENE = 0; //von 1-6
        //virtual void checkButtons(void)=0;
        //virtual void udpprint(String text) = 0;

        void begin_A(std::shared_ptr<MiLightUdpServer> svr);
        void setLightOn(uint8_t gruppe);
        void setLightOff(uint8_t gruppe);
        void setLightonoff(uint8_t gruppe, uint8_t wert);
        void setNightmode(uint8_t gruppe);
        void setWhitemode(uint8_t gruppe);
        void setColor(uint8_t gruppe, uint8_t color);
        void setSaturation(uint8_t gruppe, uint8_t sat);
        void setBrightness(uint8_t gruppe, uint8_t bright);
        void setKelvin(uint8_t gruppe, uint8_t kel);
        void setProgram(uint8_t gruppe, uint8_t prog);
        //void setMode(uint8_t gruppe, uint8_t mode);
        void setSpeed(uint8_t gruppe, int speed);
        void speedup(uint8_t gruppe);
        void speeddown(uint8_t gruppe);
        void link(uint8_t gruppe);
        void unlink(uint8_t gruppe);

        /*void saveActive(u8 scene, bool wert);
        void saveOn(u8 scene, u8 group, bool wert);
        void saveNight(u8 scene, u8 group, bool wert);
        void saveWhite(u8 scene, u8 group, bool wert);
        void saveColor(u8 scene, u8 group, u8 wert);
        void saveSat(u8 scene, u8 group, u8 wert);
        void saveBrigt(u8 scene, u8 group, u8 wert);
        void saveKel(u8 scene, u8 group, u8 wert);
        void saveProg(u8 scene, u8 group, u8 wert);
        void saveSpeed(u8 scene, u8 group, int wert);
        void doLink(u8 group);
        void doUnlink(u8 group);*/
        //void listenSerial(void);

        bool saveScene(uint8_t sc);
        bool saveGroup(uint8_t sc, uint8_t gr);
        bool saveTime(uint8_t sc);

        //void checkScene(uint8_t *scmd); // momentan nicht benötigt?
        void setNTPtime(time_t timestamp);

        bool check_scene_time(ulong aktsec, uint8_t sc, bool isWinter); // prüfen, ob scene aktiv ist
        uint8_t selectScene(void); // check all scenes to see which one schould be loaded and load it, if mode==auto
        void activateScene(uint8_t sc);
        void allOff(void);
        void testEEPROM(void);
        void ClearEEPROM(void);

        uint8_t scenemode = MODE_MANUAL;
        //void runled(void);
        //void checkButs(uint8_t *scm);
        ButtonsX *buts;
        

  
        
        

        // Wenn an den Structs irgendwas geändert wird, muss in SaveToEEPROM.h adaptiert werden!!!!
        // wegen Speicher auf 8 Gruppen und 6 Scenen limitiert. 
        // max. Anzahl an Gruppen und scenen in und Interaction.h definiert. 


        
        //const uint8_t MAX_GROUPS = 8; // active + 2 Time + 4 Gruppen + Group Pointer Array
        //const uint8_t MAX_SCENES = 6; // hier unten und in SaveToEEPROM.h auch anpassen, wenn mehrere Scenen gehen sollten. Mehr als 5 gehen aber speichertechnisch nicht
        
        //const uint8_t SIZE_GROUP = 10; // col, white, night, sat, bright, kelvin, prog, on, speed(int16) 
        //const uint8_t SIZE_TIME = 4;
        //onst uint8_t SIZE_SCENE = 1+2*SIZE_TIME+MAX_GROUPS*SIZE_GROUP; // active + 2 Time + 4 Gruppen 
        //const uint8_t GROUP_OFFSET = 1+SIZE_TIME*2; //active + Sommer+Winter 
        //const uint8_t START_OFFSET = 10;

        struct GROUP{
            //uint8_t id=1; // wird nicht mitgespeichert
            uint8_t color = 0;
            bool white=false;
            bool night = false;
            uint8_t sat=0;
            uint8_t bright=0;
            uint8_t kelvin=0;
            uint8_t prog=0;
            bool on = false;  
            uint8_t speed = 0; // siehe main file ganz oben. 0-10, -1=11, -2==12 usw.
        };
        struct TIME{
            // <stunde>:<minute>
            uint8_t starth;
            uint8_t startmin;
            // <stunde>:<minute>
            uint8_t stoph;
            uint8_t stopmin;  
        };
        struct SCENE{
            bool active;
            TIME sommer;
            TIME winter;
            GROUP gruppe1;
            GROUP gruppe2;
            GROUP gruppe3;
            GROUP gruppe4;
            GROUP gruppe5;
            GROUP gruppe6;
            GROUP gruppe7;
            GROUP gruppe8;
            GROUP *gruppen[8] = {&gruppe1, &gruppe2, &gruppe3, &gruppe4, &gruppe5, &gruppe6, &gruppe7, &gruppe8};
            //GROUP *gruppen[4] = {&gruppe1, &gruppe2, &gruppe3, &gruppe4};
        };

        SCENE scene1;
        //SCENE *scenes[1] = {&scene1};
        SCENE scene2;
        SCENE scene3;
        SCENE scene4;
        SCENE scene5;
        SCENE scene6;
        SCENE *scenes[6] = {&scene1, &scene2, &scene3, &scene4, &scene5, &scene6};
        //SCENE scene5;
        //SCENE scene6;
        //SCENE scene7;
        //SCENE scene8;
        //SCENE *scenes[8] = {&scene1, &scene2, &scene3, &scene4, &scene5, &scene6, &scene7, &scene8};

        
        //TestKlasse *test;
        bool initScenes(uint8_t go);

    private:
        std::shared_ptr<MiLightUdpServer> udpserver = nullptr;
        uint8_t command_repeat = 1; // anzahl der resends
        uint8_t reptime = 1; // delay zwischen den resends
        uint8_t sequence_id = 0;
        //uint8_t mi_command[22] = {0x80,0x00,0x00,0x00,0x11,0x00,0x01,0x01,0x01,0x00,0x31,0x00,0x00,0x8,0x04,0x02,0x00,0x00,0x00,0x01,0x00,0x63};
        uint8_t mi_command[22] = {0x80,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x01,0x00,0x31,0x00,0x00,0x8,0x04,0x02,0x00,0x00,0x00,0x01,0x00,0x63};
        void sendCommand(void);
        void checksum(void);
        
        //uint64_t getSessionID(void);   

        //EEPROM Operationen
        bool loadScene(uint8_t sc);
        bool loadGroup(uint8_t sc, uint8_t gp);
        void loadAllScenes(void);
        
        
        unsigned long epoch = 0; 
        GlobalStorage *store;
        
        

};

#endif