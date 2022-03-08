

#ifndef _BUTTONSX_H
#define _BUTTONSX_H

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include "cmddefinitions.h"
#include "globalStorage.cpp"



class ButtonsX{

    public: 
        ButtonsX();
        ButtonsX(GlobalStorage *gs=nullptr);
        void begin(void);
        void checkLeds(bool manual);
        void debounce(void);
        uint8_t leds[8] = {LED_AUTO, LED_SC1, LED_SC2, LED_SC3, LED_SC4, LED_SC5, LED_SC6, LED_TIME};
        void autoblink(void);
        void AutoBusy(bool busy);
        void wlanstatus(bool ok);
        void timestatus(bool ok);
        ~ButtonsX();

    private:
        Adafruit_MCP23017 mcp;
        uint8_t active_scene = 0;
        uint8_t scene_mode = 0;
        uint8_t selected_scene = 0;
        unsigned long pre_auto = 0;
        unsigned long pre_scene = 0;
        unsigned long inter = 400;
        bool state_scene = false;
        bool state_auto = false;
        uint8_t scene = 1;
        uint8_t autos = 1;
        uint8_t scanI2C(void);
        unsigned long pre_blink = 0;
        unsigned long inter_blink = 5000;
        GlobalStorage *store;

};



#endif