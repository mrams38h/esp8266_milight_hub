

#ifndef GLOBALSTORAGE_CPP
#define GLOBALSTORAGE_CPP

#include <Arduino.h>
#include <string.h>
#include "cmddefinitions.h"

class GlobalStorage{


public:
    
    // die 3 scene variablen sind für die aktive und zu ladende scene sowie den moed verantwortlich
    uint8_t SELECTED_SCENE = 0;
    uint8_t ACTIVE_SCENE = 0;
    uint8_t SCENE_MODE = MODE_AUTO;

    uint8_t should_load = 0; // 1 = scene ausgewählt, 2=scene bestätigt, 0= idel; wird durch button_scene auf true geseetzt, und durch button_auto bestätign/false
    //bool led_loading = false; // wird durch activateScene auf true gessetzt
    time_t newNTPtime = 0; // hier wird die pc zeit gespeichert
    bool newtime = false; // wird gesetzt, wenn die zeit manuell durch das pc programm gesetzt wurde
    time_t newCalcTime = 0; //hier wird die zeit(sommer winter) von ntptime hingegeben
    bool isWinter = false; // wird von ntptime checkWinter gesetzt
    uint8_t debug = 0; // wird von interUPD durch pc input gesetzt. wenn tire, dann werden infos an udp ausgegeben
    bool busy = false; // wird von loadScene gesetzt und für das led zum ladestatus verwendet
    bool inSync = false; // Time synchronisiert? wird von den LEDs benötigt wegen led 5 und 6
    uint8_t shouldResetDST = 0; // Wird durch got_uinix_timestamp udp message gesetzt. Wird in checkWinter verarbeitet und wieder auf 0 gesetzt.
    uint16_t SessionIDcapered = 1; // Session id aus V6MiLightUdpServer geholt und in Interaction verwendet. Deaktiviertz in V6MilightServer.cpp
    bool manualScenePC = false; // Scene vom PC manuell gewählt. wird von interUDP->got mode command gesertzt und von Interaction->selecteScene wieder entfernt


    byte on_bit_1 = 0x08; // Alle ON/OFF Commands (night, white, on, off, onoff
    byte on_bit_2 = 0x04; // Alle ON/OFF Commands (night, white, on, off, onoff
    byte on_bit_3 = 0x01; // nur beim on command in setLightOn und setLigtonoff

};

#endif