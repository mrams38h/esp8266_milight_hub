#include <Interaction.h>


// Komplette PUDP Protokollbeschreibung
// https://github.com/BKrajancic/LimitlessLED-DevAPI/blob/master/Wifi%20Bridge%20v6%20Protocol.md

// Diese ganze Projekt:
// https://github.com/sidoh/esp8266_milight_hub

Interaction::Interaction(GlobalStorage *gs)
{
  store = gs;
  buts = new ButtonsX(store);
  buts->begin();
  //TestKlasse *test = new TestKlasse(">Testklasse<");
  
}

/*Interaction::~Interaction(void)
{
  
}*/

// EEPROM wird nur bei Load und Save funktions direkt gestartet und beendet!

void Interaction::begin_A(std::shared_ptr<MiLightUdpServer> svr)
{
    //Serial.println("Start of begin");
    initScenes(0);
    //Serial.println(F("Scenes Init"));
    udpserver = svr;
    //testEEPROM(); // nur um zu schauen, ob der originalsketch den eeprom nutzt -> nein
    Serial.println(F("Scenes Inited and Tested"));
    mi_command[6] = store->SessionIDcapered; // 5->SessionID 1, 6 SessionID2. Muss getestet werden.
    //Serial.println("Stole Session ID "+String(store->SessionIDcapered,HEX));
    
}



void Interaction::setNTPtime(time_t timestamp){
  setTime(timestamp);

  epoch = timestamp;

}


void Interaction::testEEPROM(void)
{
  EEPROM.begin(512);
  Serial.println(F("### BEGIN OF 512 EEPROM ###"));
  uint8_t breaker = 0;
for(int a = 0; a<512; a++)
{
    uint8_t x = EEPROM.read(a);
    Serial.print(String(x)+" ");
    breaker++;
    if(breaker==16){Serial.println(""); breaker = 0;}
}
Serial.println(F("### END OF 512 EEPROM ###"));
EEPROM.end();
}

void Interaction::ClearEEPROM(void)
{

  EEPROM.begin(512);
  for(uint8_t i=0; i<512; i++)
  {
    EEPROM.write(i,0);
  }
  EEPROM.commit();
  EEPROM.end();
  ESP.reset();
}

void Interaction::checksum(void)
{
    int temp = 0;
    for(uint8_t i=9; i<19; i++)
    {
        temp = temp+mi_command[i];
      }   
      mi_command[20] = (temp >> 8) & 0xFF;
      mi_command[21] = temp & 0xFF;
}

void Interaction::sendCommand(void)
{
  if(sequence_id < 255){
    sequence_id++;
    }
    else{
      sequence_id = 0;
      //(running < 255) ? running++ : running = 0;
    }
  mi_command[8] = sequence_id;
  mi_command[6] = store->SessionIDcapered; // 5->SessionID 1, 6 SessionID2. Muss getestet werden.
  //mi_command[7] = running;

  checksum();
  
  for(uint8_t i = 0; i < command_repeat; i++)
  {
      udpserver->handlePacket(&mi_command[0], 22);
      delay(reptime);
    } 
}

void Interaction::setLightonoff(uint8_t gruppe, uint8_t wert)
 {
  mi_command[19] = gruppe;
  mi_command[13] = store->on_bit_1;
  mi_command[14] = store->on_bit_2;
  if(wert==1){
  mi_command[15] = store->on_bit_3;
  Serial.println(F("ON"));
  }
  else{
    mi_command[15] = 0x02;
    Serial.println(F("OFF"));
  }
  
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  
  sendCommand();
  
  }

void Interaction::setLightOn(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = store->on_bit_1;
  mi_command[14] = store->on_bit_2;
  mi_command[15] = store->on_bit_3;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println(F("ON"));
  sendCommand();
  
  }

void Interaction::setLightOff(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = store->on_bit_1;
  mi_command[14] = store->on_bit_2;
  mi_command[15] = 0x02;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println(F("OFF"));
  sendCommand();
  }

void Interaction::setNightmode(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = store->on_bit_1;
  mi_command[14] = store->on_bit_2;
  mi_command[15] = 0x05;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println(F("Night"));
  sendCommand();
  }

void Interaction::setWhitemode(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = store->on_bit_1;
  mi_command[14] = store->on_bit_2;
  mi_command[15] = 0x40;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println(F("White"));
  sendCommand();
  }

void Interaction::setColor(uint8_t gruppe, uint8_t color)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x01;
  mi_command[15] = color;
  mi_command[16] = color;
  mi_command[17] = color;
  mi_command[18] = color;
  mi_command[10] = 0x31;
  Serial.println("Col "+String(color));
  sendCommand();
  }

void Interaction::setSaturation(uint8_t gruppe, uint8_t sat)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x02;
  mi_command[15] = sat;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println("Sat "+String(sat));
  sendCommand();
  }
void Interaction::setBrightness(uint8_t gruppe, uint8_t bright)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x03;
  mi_command[15] = bright;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println("Bright "+String(bright));
  sendCommand();
  }
void Interaction::setKelvin(uint8_t gruppe, uint8_t kel)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x05;
  mi_command[15] = kel;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println("Kel "+String(kel));
  sendCommand();
  }

 void Interaction::setProgram(uint8_t gruppe, uint8_t mode)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x06;
  mi_command[15] = mode;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println("Mode "+String(mode));
  sendCommand();
  }

void Interaction::setSpeed(uint8_t gp, int speed)
{



    if(speed>0 && speed <=10)
    {
        for(uint8_t i = 0; i<speed; i++)
        {
          speedup(gp);
        }
    }
    else
    if(speed>10 && speed < 21){
      speed = speed-10;
        for(uint8_t i = 0; i<speed; i++)
        {
          speeddown(gp);
        }
    }
}

void Interaction::speeddown(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x04;
  mi_command[15] = 0x04;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println(F("Down"));
  sendCommand();
  }

void Interaction::speedup(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x04;
  mi_command[15] = 0x03;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x31;
  Serial.println(F("Up"));
  sendCommand();
  }

void Interaction::link(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x00;
  mi_command[15] = 0x00;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x3D;
  //Serial.println(F("Ready for linking in 10 Sekunden..."));
  /*for(u8 i=10; i>0; i--)
  {
    delay(1000);
    Serial.print(String(i)+" ");

  }*/
 // Serial.println(F("Go!"));
  //for(uint8_t z=0; z<10; z++)
 // {
  sendCommand();
  //delay(600);
  //}
  //Serial.println(F("Link Done"));
  }
  

void Interaction::unlink(uint8_t gruppe)
 {
  mi_command[19] = gruppe;
  mi_command[13] = 0x08;
  mi_command[14] = 0x00;
  mi_command[15] = 0x00;
  mi_command[16] = 0x00;
  mi_command[17] = 0x00;
  mi_command[18] = 0x00;
  mi_command[10] = 0x3E;
  Serial.println(F("Ready for UNLinking in 10 Sekunden..."));
  /*for(u8 i=10; i>0;i--)
  {
    delay(1000);
    Serial.print(String(i)+" ");

  }*/
  //Serial.println(F("Go!"));
  //for(uint8_t z=0; z<10; z++)
  //{
  sendCommand();
  //delay(600);
  //}
  //Serial.println(F("UN Link Done"));
  }

  

  bool Interaction::initScenes(uint8_t go = 0)
{
    
    
      uint8_t sad = ADD_KEY;
      uint8_t key = KEY_VAL;

      if(go==0){
      EEPROM.begin(521);
      uint8_t x = EEPROM.read(sad);
      EEPROM.end();

      Serial.println("Read: "+String(x)+" Key: "+String(key));
      if(x==key){
        
        Serial.println(F("### Key found. Loading! ###"));
        loadAllScenes();
         return true;
      }
    
    Serial.println(F("### Key NOT found. Initializing! ###"));
    }

  for(uint8_t i=0; i<MAX_SCENES; i++)
  {
    uint8_t col = 0;
    scenes[i]->active = true;
    scenes[i]->sommer.starth = 6+(i*2);
    scenes[i]->sommer.startmin = 15;
    scenes[i]->sommer.stoph = 7+(i*2);
    scenes[i]->sommer.stopmin = 45;

    scenes[i]->winter.starth = 7+(i*2);
    scenes[i]->winter.startmin = 30;
    scenes[i]->winter.stoph = 8+(i*2);
    scenes[i]->winter.stopmin = 58;

    for(uint8_t j=0; j<MAX_GROUPS; j++)
    {
        //scenes[i]->gruppen[j]->id=j+1;
        scenes[i]->gruppen[j]->on=true;
        scenes[i]->gruppen[j]->color=col+28;
        scenes[i]->gruppen[j]->white=false;
        scenes[i]->gruppen[j]->night=false;
        scenes[i]->gruppen[j]->sat=0;
        scenes[i]->gruppen[j]->bright=50;
        scenes[i]->gruppen[j]->kelvin=0;
        scenes[i]->gruppen[j]->prog=0;
        scenes[i]->gruppen[j]->speed=0;
     }

  }
  scenes[4]->active = false;
  scenes[4]->active = false;
 
 for(uint8_t i=0; i<MAX_SCENES; i++)
  {
    saveScene(i+1); 
  }

  if(go==0){
  EEPROM.begin(512);
  EEPROM.write(sad,key);
  EEPROM.commit();
  EEPROM.end();
  return false;
  } else
    {return true;}
    return false;
  }

bool Interaction::saveTime(uint8_t sc)
{
  EEPROM.begin(512);
if(sc <= 0 || sc > MAX_SCENES){Serial.println(F("Scene out of range!")); return false;}
        int add_SC = (ADD_OFFSET+SIZE_SCENE*(sc-1));
        EEPROM.write(add_SC, scenes[sc-1]->active);
        EEPROM.write(add_SC+1, scenes[sc-1]->sommer.starth);
        EEPROM.write(add_SC+2, scenes[sc-1]->sommer.startmin);
        EEPROM.write(add_SC+3, scenes[sc-1]->sommer.stoph);
        EEPROM.write(add_SC+4, scenes[sc-1]->sommer.stopmin);
        EEPROM.write(add_SC+5, scenes[sc-1]->winter.starth);
        EEPROM.write(add_SC+6, scenes[sc-1]->winter.startmin);
        EEPROM.write(add_SC+7, scenes[sc-1]->winter.stoph);
        EEPROM.write(add_SC+8, scenes[sc-1]->winter.stopmin);
        delay(1);
        EEPROM.commit();
    EEPROM.end();
        return true;
}

bool Interaction::saveGroup(uint8_t sc, uint8_t gr)
{
  EEPROM.begin(512);
        if(sc <= 0 || sc > MAX_SCENES){Serial.println(F("Scene out of range!")); return false;}
        if(gr <= 0 || gr > MAX_GROUPS){Serial.println(F("Group out of range!")); return false;}
        
        int add = (ADD_OFFSET+SIZE_SCENE*(sc-1)) + GROUP_OFFSET+(gr-1)*SIZE_GROUP+1; // 1 weil 1. feld ID ist, die constatnt bleibt
        EEPROM.write(add, scenes[sc-1]->gruppen[gr-1]->color);
        EEPROM.write(add+1, scenes[sc-1]->gruppen[gr-1]->white);
        EEPROM.write(add+2, scenes[sc-1]->gruppen[gr-1]->night);
        EEPROM.write(add+3, scenes[sc-1]->gruppen[gr-1]->sat);
        EEPROM.write(add+4, scenes[sc-1]->gruppen[gr-1]->bright);
        EEPROM.write(add+5, scenes[sc-1]->gruppen[gr-1]->kelvin);
        EEPROM.write(add+6, scenes[sc-1]->gruppen[gr-1]->on);
        EEPROM.write(add+7, scenes[sc-1]->gruppen[gr-1]->prog);
        EEPROM.write(add+8, scenes[sc-1]->gruppen[gr-1]->speed);
        //Serial.println("EEPROM SAVE Speed: "+String(scenes[sc-1]->gruppen[gr-1]->speed));
        EEPROM.commit();
        Serial.println("Saved Group "+String(gr)+" in scene "+String(sc));
      EEPROM.end();   
        return true;
}

bool Interaction::saveScene(uint8_t sc)
{
  
        if(sc <= 0 || sc > MAX_SCENES){Serial.println(F("Scene out of range!")); return false;}
        saveTime(sc);

        for(uint8_t i=0; i< MAX_GROUPS; i++)
        {
            saveGroup(sc, (i+1)); // scenen und gruppen beginnen im funktionsaufrug immmer mit 1
            delay(1);
        }
        
        Serial.println("Saved Scene! "+String(sc));
    
        return true;
}

bool Interaction::loadGroup(uint8_t sc, uint8_t gp){
  EEPROM.begin(512);
      if(gp <= 0 || gp > MAX_GROUPS){Serial.println(F("Group out of range!")); return false;}
      int add = (ADD_OFFSET+SIZE_SCENE*(sc-1)) + GROUP_OFFSET+(gp-1)*SIZE_GROUP+1; // 1 weil 1. feld ID ist, die constatnt bleibt
      scenes[sc-1]->gruppen[gp-1]->color = EEPROM.read(add);
      scenes[sc-1]->gruppen[gp-1]->white = EEPROM.read(add+1);
      scenes[sc-1]->gruppen[gp-1]->night = EEPROM.read(add+2);
      scenes[sc-1]->gruppen[gp-1]->sat = EEPROM.read(add+3);
      scenes[sc-1]->gruppen[gp-1]->bright = EEPROM.read(add+4);
      scenes[sc-1]->gruppen[gp-1]->kelvin = EEPROM.read(add+5);
      scenes[sc-1]->gruppen[gp-1]->on = EEPROM.read(add+6);
      scenes[sc-1]->gruppen[gp-1]->prog = EEPROM.read(add+7);
      scenes[sc-1]->gruppen[gp-1]->speed = EEPROM.read(add+8);
      //Serial.println("EEPROM READ Speed: "+String(scenes[sc-1]->gruppen[gp-1]->speed));
  EEPROM.end();
      //Serial.println("Loaded Group "+String(gp));
      return true;

      

}

bool Interaction::loadScene(uint8_t sc)
{
  
  if(sc <= 0 || sc > MAX_SCENES){Serial.println(F("Scene out of range!")); return false;}

        int add_SC = (ADD_OFFSET+SIZE_SCENE*(sc-1));
        EEPROM.begin(512);
        scenes[sc-1]->active = EEPROM.read(add_SC);
        scenes[sc-1]->sommer.starth = EEPROM.read(add_SC+1);
        scenes[sc-1]->sommer.startmin = EEPROM.read(add_SC+2);
        scenes[sc-1]->sommer.stoph = EEPROM.read(add_SC+3);
        scenes[sc-1]->sommer.stopmin = EEPROM.read(add_SC+4);
        scenes[sc-1]->winter.starth = EEPROM.read(add_SC+5);
        scenes[sc-1]->winter.startmin = EEPROM.read(add_SC+6);
        scenes[sc-1]->winter.stoph = EEPROM.read(add_SC+7);
        scenes[sc-1]->winter.stopmin = EEPROM.read(add_SC+8);
        EEPROM.end();
        for(uint8_t i=0; i< MAX_GROUPS; i++)
        {
            loadGroup(sc, (i+1)); // scenen und gruppen beginnen im funktionsaufrug immmer mit 1
            //delay(1);
        }
        
        //Serial.println("Loaded Scene! "+String(sc));   
        return true;

}
  
void Interaction::loadAllScenes(void){
for(uint8_t i=0; i<MAX_SCENES; i++)
{
    loadScene(i+1);
    //delay(1);
}
Serial.println("Loaded all "+String(MAX_SCENES)+" Scenes!");
}

void Interaction::activateScene(uint8_t sc)
{
Serial.println("Load Szene "+String(sc));
buts->AutoBusy(true);
for(uint8_t gp=1; gp<=MAX_GROUPS; gp++){ 
   if(scenes[sc-1]->gruppen[gp-1]->on==false){
     setLightOff(gp);
     }else
   if(scenes[sc-1]->gruppen[gp-1]->night==true) // Nightmode braucht nicht mehr 
   {
     setLightOn(gp);
     setNightmode(gp);
   }
   else
   if(scenes[sc-1]->gruppen[gp-1]->white==true) //whitemode braucht nur white, bright und kelvin
   {
     setLightOn(gp);
     setWhitemode(gp);
     setBrightness(gp,scenes[sc-1]->gruppen[gp-1]->bright);
     setKelvin(gp,scenes[sc-1]->gruppen[gp-1]->kelvin);
     setSaturation(gp,scenes[sc-1]->gruppen[gp-1]->sat);
   } else
   if(scenes[sc-1]->gruppen[sc-1]->prog!=0)
   {
      setLightOn(gp);
      setProgram(gp,0);
      setBrightness(gp,scenes[sc-1]->gruppen[gp-1]->bright);
      setProgram(gp,scenes[sc-1]->gruppen[gp-1]->prog);
      setSpeed(gp,scenes[sc-1]->gruppen[gp-1]->speed);
      setLightOn(gp);
   }
   else{
      setLightOn(gp);
      setColor(gp,scenes[sc-1]->gruppen[gp-1]->color);
      setSaturation(gp,scenes[sc-1]->gruppen[gp-1]->sat);
      setBrightness(gp,scenes[sc-1]->gruppen[gp-1]->bright);
   }
}
buts->AutoBusy(false);
}

 void Interaction::allOff(void)
 {
   setLightOff(0);
   //SCENE_MODE = MODE_OFF; 
 } 


uint8_t Interaction::selectScene(void)
{
  uint8_t retval = 0;
  if(store->SCENE_MODE == MODE_AUTO){
    time_t act_time = now(); // aktuellen unix timestamp
    uint8_t h  = hour(act_time);
    uint8_t min  = minute(act_time);
    ulong daysec = h*3600+min*60;
    uint8_t sel_sce = 0;

    for(uint8_t i = 1; i<=MAX_SCENES; i++)
    {
        if(check_scene_time(daysec, i, store->isWinter)==true){sel_sce = i;} 
    }

    if(sel_sce > 0 && sel_sce <= MAX_SCENES)
    {
        if(scenes[sel_sce-1]->active==true){ // Wenn active = true
          if(store->ACTIVE_SCENE != sel_sce){
              store->ACTIVE_SCENE = sel_sce;
              Serial.println("Scene "+String(sel_sce)+" wird geladen!");
              activateScene(sel_sce);
          }  
          //Serial.println("Scene "+String(sel_sce)+" aktiv!");
          retval= sel_sce;
        } else{
          if(store->ACTIVE_SCENE != sel_sce){
            Serial.println("Gewählte Scene "+String(sel_sce)+"inaktiv!");
              store->ACTIVE_SCENE = sel_sce;
              allOff();
              retval=0;
          }
        else{retval = 99;} // 99 macht check scenen auf langsam stellen
        }
    }else{
      
      if(store->ACTIVE_SCENE != 0){
      Serial.println(F("Keine aktive Szene gefunden!"));
      store->ACTIVE_SCENE = 0;
      allOff();
      retval =  99; // 99 macht check scenen auf langsam stellen
      }
    } 
} else
if(store->SCENE_MODE==MODE_OFF)
{
        
        //if(store->ACTIVE_SCENE != 0){
        allOff();
        store->ACTIVE_SCENE = 0;
        
        //}
        //store->SCENE_MODE = MODE_OFF;
        retval =  99;   
} else
if(store->SCENE_MODE == MODE_MANUAL)
{
  if(store->should_load==2 || store->manualScenePC==true){
    //Serial.println("Should Load(3) = "+String(store->should_load));
    store->should_load = 0;
    store->manualScenePC=false;
    //Serial.println("Should Load(4) = "+String(store->should_load));
    Serial.println("Sel: "+String(store->SELECTED_SCENE)+" Active "+String(store->ACTIVE_SCENE));
    if(store->SELECTED_SCENE != store->ACTIVE_SCENE)
    {
      store->ACTIVE_SCENE = store->SELECTED_SCENE;
      activateScene(store->SELECTED_SCENE);
      //Serial.println("SELCTED SCNE: "+String(store->SELECTED_SCENE)+" Active Scene: "+String(store->ACTIVE_SCENE)+" Mode: "+String(store->SCENE_MODE));
      retval =  store->ACTIVE_SCENE; 
    }
  }
}
  //return store->ACTIVE_SCENE;
  return retval;
}

uint8_t offcounter = 0;

bool Interaction::check_scene_time(ulong aktsec, uint8_t sc, bool isWin) 
{
  // akttime ist zeit seit heute 00:00 in sekunden
  // don't use scenes longer than 23h!!!

bool active = false;

uint8_t sh = 8;
uint8_t sm = 0;
uint8_t eh = 12;
uint8_t em = 0;

if(isWin==true)
{
  sh = scenes[sc-1]->winter.starth;
  sm = scenes[sc-1]->winter.startmin;
  eh = scenes[sc-1]->winter.stoph;
  em = scenes[sc-1]->winter.stopmin;
}
if(isWin==false)
{
  sh = scenes[sc-1]->sommer.starth;
  sm = scenes[sc-1]->sommer.startmin;
  eh = scenes[sc-1]->sommer.stoph;
  em = scenes[sc-1]->sommer.stopmin;
}


bool over = false;
const ulong dreiundzwanzig = 86399; //23:59:59

ulong startsec = sh*3600+sm*60;
ulong stopsec = eh*3600+em*60;

(eh<sh) ? over = true : over = false;

if(over == false){
  if(aktsec > startsec && aktsec<=stopsec)
  {
    active = true;
  }
}
if(over == true)
{
  bool tp1 = false;
  bool tp2 = false;
  // Wenns über null geht, dann eine minute kein licht -> fehler
  (startsec < aktsec && startsec <= dreiundzwanzig) ? tp1 = true : tp1 = false;
  (aktsec >= 0 && stopsec>aktsec) ? tp2 = true : tp2 = false;
  (tp1==true || tp2 == true) ? active = true : active = false ;
}

return active;
}


/*uint64_t Interaction::getSessionID(void)
  {
    uint64_t ret = 0;
        //byte mise[27]={0x20,0x00,0x00,0x00,0x16,0x02,0x62,0x3A,0xD5,0xED,0xA3,0x01,0xAE,0x08,0x2D,0x46,0x62,0x41,0xA7,0xF6,0xDC,0xAF,0xD3,0xE6,0x00,0x00,0x1E};
   
        Serial.println(F("SessionID: "));
        sendCommand();
    return ret;
  }*/


  /*void Interaction::saveActive(u8 scene, bool wert){
      scenes[scene-1]->active=wert;
  }

  void Interaction::saveOn(u8 scene, u8 group, bool wert){
      scenes[scene-1]->gruppen[group-1]->on=wert;
  }

  void Interaction::saveNight(u8 scene, u8 group, bool wert){
      scenes[scene-1]->gruppen[group-1]->night=wert;
  }

  void Interaction::saveWhite(u8 scene, u8 group, bool wert){
      scenes[scene-1]->gruppen[group-1]->white=wert;
  }

  void Interaction::saveColor(u8 scene, u8 group, u8 wert){
      scenes[scene-1]->gruppen[group-1]->color=wert;
  }

  void Interaction::saveSat(u8 scene, u8 group, u8 wert){
      scenes[scene-1]->gruppen[group-1]->sat=wert;
  }

  void Interaction::saveBrigt(u8 scene, u8 group, u8 wert){
      scenes[scene-1]->gruppen[group-1]->bright=wert;
  }
  void Interaction::saveKel(u8 scene, u8 group, u8 wert){
      scenes[scene-1]->gruppen[group-1]->kelvin=wert;
  }
  void Interaction::saveProg(u8 scene, u8 group, u8 wert){
      scenes[scene-1]->gruppen[group-1]->prog=wert;
  }
  void Interaction::saveSpeed(u8 scene, u8 group, int wert){
      scenes[scene-1]->gruppen[group-1]->speed=wert;
  }
  void Interaction::doLink(u8 group){
      link(group);
  }

  void Interaction::doUnlink(u8 group){
      unlink(group);
  }*/

/*void Interaction::runled(void)
{
    buts->autoblink();
}

void Interaction::checkButs(uint8_t *scm)
{
    buts->debounce(scm);
}*/

/*void Interaction::checkScene(uint8_t *scmd){
    // Prüfen, welche szene gerade geladen werden soll

  // aktuelle zeit als h und minuten und tag monat jahr holen
  // aktuelle zeit als timestamp holen
  // timstamp aus beginn zeit holen und generieren mit aktuellem datum
  // timstamp aus end zeit holen und generieren mit aktuellem datum
  // wenn ende < anfang dann tag +1
  // entwerder winter oder sommer mit isWinter
  store->ACTIVE_SCENE = scmd[0];
  store->SCENE_MODE = scmd[1];
  time_t no = now();
  //uint8_t aday = day(no);
  //uint8_t amonth = month(no);
  //uint8_t ayear = year(no);
  uint8_t ahour = hour(no);
  uint8_t aminute = minute(no);
  //uint8_t aweekday = weekday(no);

  //Serial.println(timeStatus());

  uint8_t ah=0, am=0, eh=0, em=0;
  bool bigger = false;
  if(store->isWinter == false){
    ah = scenes[0]->sommer.starth;
    am = scenes[0]->sommer.startmin;
    eh = scenes[0]->sommer.stoph;
    em = scenes[0]->sommer.stopmin;
  }
  else{
    ah = scenes[0]->winter.starth;
    am = scenes[0]->winter.startmin;
    eh = scenes[0]->winter.stoph;
    em = scenes[0]->winter.stopmin;
    }
  if(eh<ah){bigger = true;}else{bigger = false;} 
  if(ah==eh){if(em<am){bigger = true;}else{bigger = false;}}

  tmElements_t myTimeElementsA, myTimeElementsE, noka;
  noka = {0,aminute,ahour,1,1,1,0};
  time_t noakt = makeTime(noka);
  //ayear = ayear-1970;
  myTimeElementsA = {0, am, ah, 1, 1, 1, 0};
  
  time_t pasta = makeTime(myTimeElementsA);
  //uint8_t dz = 0;
  //if(bigger ==true){dz=2;}
  myTimeElementsE = {0, em, eh, 1, 1, 1, 0 };
  time_t futura = makeTime(myTimeElementsE);

  Serial.println("Akt Time: "+String(noakt)+" ("+String(hour(noakt))+":"+String(minute(noakt))+":"+String(second(noakt))+")");
  Serial.println("Start Time: "+String(pasta)+" ("+String(ah)+":"+String(am)+")");
  Serial.println("End Time: "+String(futura)+" ("+String(eh)+":"+String(em)+")");

  Serial.println("noakt("+String(noakt)+") > pasta("+String(pasta)+") && noakt("+String(noakt)+") <= futura("+String(futura)+")");
  if(bigger==false){
    if(noakt > pasta && noakt <= futura)
    {
      Serial.println("Action in Timerange!");
    }
  }
  else{
    if(noakt > futura && noakt <= pasta)
    {
      Serial.println("Action in Timerange!");
    }
  }
if(scenemode == MODE_MANUAL){
  Serial.println("Manual Mode\r\n");


}else
if(scenemode== MODE_AUTO){
  Serial.println("Auto Mode\r\n");
}else{
  Serial.println("Manual Mode\r\n");

}

scmd[0] = store->ACTIVE_SCENE;
scmd[1] = store->SCENE_MODE;

}*/

