



#define DEBUG_BUTTONS



// defines sind warscheinlich im Flash gespeichert. 

    #define MAX_SCENES 6
    #define MAX_GROUPS 8
    #define SIZE_GROUP 9 // col, white, night, sat, bright, kelvin, prog, on, speed
    #define SIZE_TIME  4
    #define SIZE_SCENE 81 //1+2*SIZE_TIME+MAX_GROUPS*SIZE_GROUP; // active + 2 Time + 4 Gruppen 
    #define GROUP_OFFSET 9 // 1+SIZE_TIME*2; //active + Sommer+Winter 
    

    #define UDP_ERROR 99 // error ist nicht gleich fehler. Einfacvh Antwort. Fehler erst, wenn 2. teil 1-3
    #define UDP_ERROR_MSG_OK 0
    #define UDP_ERROR_MSG_GROUP 1
    #define UDP_ERROR_MSG_SCENE 2
    #define UDP_ERROR_MSG_PARAM 3
    #define UDP_ERROR_MSG_UNDEFINED 9
    #define UDP_ERROR_MSG_CHECKUM 10
    
    #define TIME_ERROR 94
    #define PARAM_ERROR 98

    #define CMD_DEBUG 96
    #define ANTW_DEBUG 97
    #define DEBUG_ON 1
    #define DEBUG_OFF 0

    #define ANFR_SET_BITS 25

    
    #define ANFR_HELLO 222
    #define HELLO_MY 2
    #define HELLO_NOTOK 0 // kann der controller theor. uach senden. wei noch nicht wo.
    #define HELLO_GET_OK 1 // bekommt der controller und antwortet mit HELLO_MY

    #define CMD_SET_TIMESTAMP 24


    //#define ANFR_CHANGE_LIGHT 15

    // Antworte, die der controller and den PC sendet (nut auf get, sonnst 99)
    #define ANTW_GET_GRUPPE 11
    #define ANTW_GET_SCENEHEADER 6
    
    // Anfragen, die der Controller vom PC bekommt und was tut und dann mit 99;0-3 antwortet
    #define ANFR_GET_GRUPPE 10
    #define ANFR_GET_SCENEHEADER 5
    #define ANFR_SET_TIME 20
    #define ANFR_SET_GROUP 23 // bekommt string mit gruppe
    #define ANFR_SAVE_SCENE 70
    #define ANFR_SAVE_GROUP 72
    #define ANFR_SAVE_TIME 74
    #define ANFR_LINK 30
    #define ANFR_UNLINK 32
    #define ANFR_CHANGE_PARAM 12
    #define ANFR_MODE 36
    #define ANFR_REBOOT 45
    //#define ANFR_CHANGE_GROUP 15

    #define ANFR_RESET 34 
    #define RESET_KEY 55

    #define ANFR_GET_AKT_TIME 18
    #define ANTW_GET_AKT_TIME 19

    // Von ANFR_MODE
    #define MODE_OFF 0
    #define MODE_MANUAL 1
    #define MODE_AUTO 2
    

    // von ANFR_CHANGE_PARAM
    #define PARAM_COUNT 11
    #define PARAM_ON 1
    #define PARAM_NIGHT 2
    #define PARAM_WHITE 3 
    #define PARAM_COLOR 4
    #define PARAM_SAT 5
    #define PARAM_BRIGHT 6
    #define PARAM_KELVIN 7
    #define PARAM_PROG 8
    #define PARAM_SPEED 9
    #define PARAM_UP 10
    #define PARAM_DOWN 11



    // ### MCP23017 PORT DEFS ###
    #define BUT_AUTO 13 //External Pullups
    #define BUT_SCENE 12 //External Pullups
    #define LED_SC1 1
    #define LED_SC2 2
    #define LED_SC3 3
    #define LED_SC4 6
    #define LED_SC5 5
    #define LED_SC6 4
    #define LED_AUTO 0
    #define LED_TIME 7


    // ### EEPROM ADDRESSES
    #define KEY_VAL 99
    #define ADD_KEY 2 // Initialized? Key -> Interaction.cpp
    #define ADD_DST 5 // Sumemr Time State -> ntptime.cpp
    #define ADD_OFFSET 10 // Start of first scene -> Interaction.cpp

    // Nach hinten ist fast kein platz mehr mit 6 Scenen und 8 Gruppen. 
    // Für genauere Checks, Interaction->testEEPROM aktivieren.

    #define UDP_CONTROL_PORT 4999 



    // MILIGHT COMMAND SETTINGS.
    // Die scheinwerfer kommen mit FUT89 und RGB_CCt zurecht. Getestet.
    // Die Normalen kommands bleiben gleich, aber die on, off, night, white commands sind anders

   /* #define RGB_CCT // FUT89, RGB_CCT

    #ifdef FUT89
        #define ON_BIT_1 0x0A
        #define ON_BIT_2 0x06
    #endif
    #ifdef RGB_CCT
        #define ON_BIT_1 0x08
        #define ON_BIT_2 0x04
    #endif*/

    
    