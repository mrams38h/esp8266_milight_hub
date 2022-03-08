




#include <interUDP.h>



InterUDP::InterUDP(GlobalStorage *gs)
{
   store = gs;
   //buts = new ButtonsX(gs);
   inter = new Interaction(gs);
  
}

InterUDP::~InterUDP(void)
{
    udpa.stop();
  
}




//void InterUDP::begin_X(NTPtime *n)
//{
//  ntp = n;
//}
void InterUDP::beginUDP(int port)
{

    
    udpa.begin(port);
    //store = gs; // in der Interaction Klasse, protected
    
}

void InterUDP::runUDP(void)
{

      int packetSize = udpa.parsePacket();
  if (packetSize) {

    int n = udpa.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    //Serial.println(packetBuffer);
    schredInput(packetBuffer);
    memset(packetBuffer,' ',57);
    
  }
}

void InterUDP::udpprint(String text)
{
    //udpa.flush();
    text = text+"\n";
    udpa.beginPacket(udpa.remoteIP(), udpa.remotePort());
    char dd[512];
    text.toCharArray(&dd[0],512);
    udpa.write(dd);
    udpa.endPacket();
    delay(10);
  }

  bool InterUDP::schredInput(char *input){

  String ta = " ";//split(input,0);
  String tb = " ";//split(input,1);
  String tc = " ";//split(input,2);
  String td = " ";
  String te = " ";
  int cmd=0, gp = 0, sc = 0, p1 = 0, p2=0, p3=0, p4=0, p5=0, p6=0, p7=0, p8=0, p9=0, param = 0, wt=0;


  ta = split(input,0);
  cmd = ta.toInt();
  //udpprint("Command is "+ta);

  if(cmd == ANFR_GET_GRUPPE) //get group
  {
        sc = split(input,1).toInt(); // szene int
        gp = split(input,2).toInt(); // gruppe int
        if(sc < 1 || sc > MAX_SCENES){udpprint(msg_sceneerror); return false;}
        if(gp < 1 || gp > MAX_GROUPS){udpprint(msg_grouperror); return false;}

            int spd = inter->scenes[sc-1]->gruppen[gp-1]->speed;
            if(spd>10 && spd<21){spd = (spd-10)*-1;}

        String ret = String(ANTW_GET_GRUPPE)+";"+String(sc)+";"+String(gp)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->on)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->night)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->white)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->color)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->sat)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->bright)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->kelvin)+";"
                    +String(inter->scenes[sc-1]->gruppen[gp-1]->prog)+";"
                    +String(spd);
        udpprint(ret);
        //Serial.println(ret);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Get Gruppe");}
        return true;
  } else 
 if(cmd == ANFR_GET_SCENEHEADER) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        if(sc < 1 || sc > MAX_SCENES){udpprint(msg_sceneerror); return false;}
        String ret = String(ANTW_GET_SCENEHEADER)+";"+String(sc)+";"
                    +String(inter->scenes[sc-1]->winter.starth)+";"
                    +String(inter->scenes[sc-1]->winter.startmin)+";"
                    +String(inter->scenes[sc-1]->winter.stoph)+";"
                    +String(inter->scenes[sc-1]->winter.stopmin)+";"
                    +String(inter->scenes[sc-1]->sommer.starth)+";"
                    +String(inter->scenes[sc-1]->sommer.startmin)+";"
                    +String(inter->scenes[sc-1]->sommer.stoph)+";"
                    +String(inter->scenes[sc-1]->sommer.stopmin)+";"
                    +String(inter->scenes[sc-1]->active)+";";
        udpprint(ret);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Get Header");}
        return true;
  } else
if(cmd==ANFR_CHANGE_PARAM)
{
  gp = split(input,1).toInt(); // group int
  param = split(input,2).toInt(); // parameter int
  wt = split(input,3).toInt(); // wert int
  if(param < 1 || param > PARAM_COUNT){udpprint(msg_paramerror); return false;}
  if(gp < 1 || gp > MAX_GROUPS){udpprint(msg_grouperror); return false;}

  if(param==PARAM_ON || param==PARAM_WHITE || param==PARAM_NIGHT){
    if(wt < 0 || wt > 1){udpprint(msg_paramerror); return false;}
  } else
  if(param==PARAM_PROG){
    if(wt < 0 || wt > 9){udpprint(msg_paramerror); return false;}
  } else
  if(param==PARAM_SPEED){
    if(wt < -10 || wt > 10){udpprint(msg_paramerror); return false;}
  } else 
  if(param==PARAM_COLOR){
    if(wt < 0 || wt > 255){udpprint(msg_paramerror); return false;}
  } 
  else{
    if(wt < 0 || wt > 100){udpprint(msg_paramerror); return false;}
  }

  switch(param)
  {
      case PARAM_ON: inter->setLightonoff(gp,wt); break; 
      case PARAM_NIGHT: inter->setNightmode(gp); break;
      case PARAM_WHITE: inter->setWhitemode(gp); break;
      case PARAM_COLOR: inter->setColor(gp,wt); break;
      case PARAM_SAT: inter->setSaturation(gp,wt); break;
      case PARAM_BRIGHT: inter->setBrightness(gp,wt); break;
      case PARAM_KELVIN: inter->setKelvin(gp,wt); break;
      case PARAM_PROG: inter->setProgram(gp,wt); break;
      case PARAM_SPEED: if(wt<0){wt = abs(wt)+10;}inter->setSpeed(gp,wt); break;
      case PARAM_DOWN: inter->speeddown(gp); break;
      case PARAM_UP: inter->speedup(gp); break;
      default: break;
  }
        
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Change Param");}
} else
 if(cmd == ANFR_HELLO) //get scene header (time and active)
  {
        
        gp = split(input,1).toInt();
        if(cmd==222 && gp==HELLO_GET_OK){udpprint(String(ANFR_HELLO)+";"+String(HELLO_MY));}else{udpprint(String(ANFR_HELLO)+";"+String(HELLO_NOTOK));}
        
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Got Hallo");}
        return true;
  } else

  if(cmd == ANFR_SET_GROUP) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        gp = split(input,2).toInt(); // szene int
        if(sc < 1 || sc > MAX_SCENES){udpprint(msg_sceneerror); return false;}
        if(gp < 1 || gp > MAX_GROUPS){udpprint(msg_grouperror); return false;}
        
        p1 = split(input,3).toInt();
        if(p1 < 0 || p1 > 1){udpprint(msg_Pparamerror+";1"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->on=p1;}
        p2 = split(input,4).toInt();
        if(p2 < 0 || p2 > 1){udpprint(msg_Pparamerror+";2"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->night=p2;}
        p3 = split(input,5).toInt();
        if(p3 < 0 || p3 > 1){udpprint(msg_Pparamerror+";3"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->white=p3;}
        p4 = split(input,6).toInt();
        if(p4<0 || p4>255){udpprint(msg_Pparamerror+";4"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->color=p4;}
        p5 = split(input,7).toInt();
        if(p5<0 || p5>100){udpprint(msg_Pparamerror+";5"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->sat=p5;}
        p6 = split(input,8).toInt();
        if(p6<0 || p6>100){udpprint(msg_Pparamerror+";6"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->bright=p6;}
        p7 = split(input,9).toInt();
        if(p7<0 || p7>100){udpprint(msg_Pparamerror+";7"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->kelvin=p7;}
        p8 = split(input,10).toInt();
        if(p8<0 || p8>9){udpprint(msg_Pparamerror+";8"); return false;}else{inter->scenes[sc-1]->gruppen[gp-1]->prog=p8;}
        p9 = split(input,11).toInt();
        if(p9<-10 || p9>10){udpprint(msg_Pparamerror+";9"); return false;}else{if(p9<0){p9 = abs(p9)+10;}inter->scenes[sc-1]->gruppen[gp-1]->speed=p9;} 
        Serial.println(F("Got SET GROUP Command"));
        inter->saveGroup(sc,gp);
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Set Gruppe");}
        return true;

  } else
  
if(cmd == ANFR_SET_TIME) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        if(sc < 1 || sc > MAX_SCENES){udpprint(msg_sceneerror); return false;}
        p1 = split(input,2).toInt();
        if(p1 < 0 || p1 > 23){udpprint(msg_Tparamerror+";1"); return false;}else{inter->scenes[sc-1]->winter.starth = p1;}
        p2 = split(input,3).toInt();
        if(p2 < 0 || p2 > 59){udpprint(msg_Tparamerror+";2"); return false;}else{inter->scenes[sc-1]->winter.startmin = p2;}
        p3 = split(input,4).toInt();
        if(p3 < 0 || p3 > 23){udpprint(msg_Tparamerror+";3"); return false;}else{inter->scenes[sc-1]->winter.stoph = p3;}
        p4 = split(input,5).toInt();
        if(p4 < 0 || p4 > 59){udpprint(msg_Tparamerror+";4"); return false;}else{inter->scenes[sc-1]->winter.stopmin = p4;}
        p5 = split(input,6).toInt();
        if(p5 < 0 || p5 > 23){udpprint(msg_Tparamerror+";5"); return false;}else{inter->scenes[sc-1]->sommer.starth = p5;}
        p6 = split(input,7).toInt();
        if(p6 < 0 || p6 > 59){udpprint(msg_Tparamerror+";6"); return false;}else{inter->scenes[sc-1]->sommer.startmin = p6;}
        p7 = split(input,8).toInt();
        if(p7 < 0 || p7 > 23){udpprint(msg_Tparamerror+";7"); return false;}else{inter->scenes[sc-1]->sommer.stoph = p7;}
        p8 = split(input,9).toInt();
        if(p8 < 0 || p8 > 59){udpprint(msg_Tparamerror+";8"); return false;}else{inter->scenes[sc-1]->sommer.stopmin = p8;}
        p9 = split(input,10).toInt();
        if(p9 < 0 || p9 > 1){udpprint(msg_Tparamerror+";9"); return false;}else{inter->scenes[sc-1]->active = p9;}
        inter->saveTime(sc);
        Serial.println(F("Got SET TIME Command"));
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Set Time");}
        return true;
  } else
  
  if(cmd == ANFR_MODE) //get scene header (time and active)
  {
        p1 = split(input,1).toInt(); // mode int
        p2 = split(input,2).toInt(); // mode int
        if(p1 < 0 || p1 > 2){udpprint(msg_paramerror); return false;}
        if(p2 < 0 || p2 > 6){udpprint(msg_sceneerror); return false;}
        
        store->ACTIVE_SCENE = 0;
        store->SCENE_MODE = p1;
        store->SELECTED_SCENE = p2;
        store->manualScenePC = true;
         Serial.println("Got MODE Command "+String(store->SCENE_MODE));  
         Serial.println("Got SCENE "+String(store->SELECTED_SCENE));               
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Set Mode");}
        return true;
  } else

 if(cmd == ANFR_LINK) //get scene header (time and active)
  {
        gp = split(input,1).toInt(); // gruppe int
        if(gp < 1 || gp > MAX_GROUPS){udpprint(msg_grouperror); return false;}
        inter->link(sc);
        Serial.println(F("Got LINK Command"));                
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Link");}
        return true;
  } else
   if(cmd == ANFR_UNLINK) //get scene header (time and active)
  {
        gp = split(input,1).toInt(); // gruppe int
        if(gp < 1 || gp > MAX_GROUPS){udpprint(msg_grouperror); return false;}
        inter->unlink(sc);
        Serial.println(F("Got UNLINK Command"));                
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Unlink");}
        return true;
  } else
   if(cmd == ANFR_REBOOT) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        if(sc != ANFR_REBOOT){udpprint(msg_paramerror); return false;}
         Serial.println(F("Got ROBOOT Command"));                
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Reboot");}
        delay(500);
        ESP.restart();
        return true;
  } else 
   if(cmd == ANFR_GET_AKT_TIME) //send TIME NOW
  {
        time_t n = now();
        String out = String(ANTW_GET_AKT_TIME)+";"+String(day(n))+";"+String(month(n))+";"+String(year(n))+";"+String(hour(n))+";"+String(minute(n))+";"+String(second(n));               
        udpprint(out);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Get Time");}
        return true;
  } else
   /*if(cmd == CMD_DEBUG) //send TIME NOW
  {
        p1 = split(input,1).toInt(); // value
        Serial.println("Debug->"+String(p1));
        if(p1 < 0 || p1 > 1){udpprint(msg_paramerror); return false;}             
        if(p1==DEBUG_ON){store->debug=true;}else{store->debug=false;}
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Got Debug");}
        return true;
  } else*/
      if(cmd == ANFR_RESET) //send TIME NOW
  {
        p1 = split(input,1).toInt(); // szene int
        if(p1 != RESET_KEY){udpprint(msg_paramerror); return false;} 
        //inter->ClearEEPROM();            
        inter->initScenes(1);
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Got Scene reset");}
        return true;
  } else
      if(cmd == CMD_SET_TIMESTAMP) //get TIME NOW
  {
        unsigned long tx = strtol(split(input,1).c_str(),NULL,0); 
        uint8_t dstres = atoi(split(input,2).c_str());

        //Serial.println("Got X Timestamp: "+String(tx));
        
        if(tx < 946681200 || tx > 2840137200){udpprint(msg_paramerror); return false;}             
        if(dstres <0 && dstres > 1){udpprint(msg_paramerror); return false;}
        store->shouldResetDST = dstres;
        //unsigned long tt = ntp->checkWinter(tx);
        //setNTPtime(tt);
        store->newNTPtime = tx;
        store->newtime = true;
        udpprint(msg_okerror);
        //Serial.println("Timestamp received = "+String(tx));
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Got Tiemstamp");}
        return true;
  } else
   if(cmd == ANFR_SET_BITS) //get scene header (time and active)
  {
        p1 = split(input,1).toInt(); // wert byte 13
        p2 = split(input,2).toInt(); // wert byte 14
        p3 = split(input,3).toInt(); // wert byte 15

        store->on_bit_1 = p1;
        store->on_bit_2 = p2;
        store->on_bit_3 = p3;
      
        Serial.println("Got BIT Command "+String(p1,HEX)+" "+String(p2,HEX)+" "+String(p3,HEX));                
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Link");}
        return true;
  } 
  return false;
}// end of schred input


String InterUDP::split(char *da, int index)
  {
    String data(da);
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;

    for(int i=0; i<=maxIndex && found<=index; i++){
      if(data.charAt(i)==';' || i==maxIndex){
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "x";
  }

/*
 if(cmd == ANFR_SET_GROUP) 
  {
        sc = split(input,1).toInt(); // szene int
        gp = split(input,2).toInt(); // szene int
        if(sc < 1 || sc > MAX_SCENES){udpprint(msg_sceneerror); return false;}
        if(gp < 1 || gp > MAX_GROUPS){udpprint(msg_grouperror); return false;}
        Serial.println(F("Got SET GROUP Scene und Group OK"));
        p1 = split(input,3).toInt();
        if(p1 != 0 || p1 != 1){udpprint(msg_paramerror); return false;}else{(p1==1) ? inter->setLightOn(gp) : inter->setLightOff(gp);}
        p2 = split(input,4).toInt();
        if(p2 != 0 || p2 != 1){udpprint(msg_paramerror); return false;}else{inter->setNightmode(gp);}
        p3 = split(input,5).toInt();
        if(p3 != 0 || p3 != 1){udpprint(msg_paramerror); return false;}else{inter->setWhitemode(gp);}
        p4 = split(input,6).toInt();
        if(p4<0 || p4>255){udpprint(msg_paramerror); return false;}else{inter->setColor(gp,p4);}
        p5 = split(input,7).toInt();
        if(p5<0 || p5>100){udpprint(msg_paramerror); return false;}else{inter->setSaturation(gp,p5);}
        p6 = split(input,8).toInt();
        if(p6<0 || p6>100){udpprint(msg_paramerror); return false;}else{inter->setBrightness(gp,p6);}
        p7 = split(input,9).toInt();
        if(p7<0 || p7>100){udpprint(msg_paramerror); return false;}else{inter->setKelvin(gp,p7);}
        p8 = split(input,10).toInt();
        if(p8<0 || p8>100){udpprint(msg_paramerror); return false;}else{inter->setProgram(gp,p8);}
        p9 = split(input,11).toInt();
        if(p9<0 || p9>100){udpprint(msg_paramerror); return false;}else{inter->setSpeed(gp,p9);} 
        inter->saveGroup(sc,gp);
        Serial.println(F("Got SET GROUP Command"));
        udpprint(msg_okerror);
        //if(store->debug==true){udpprint(String(ANTW_DEBUG)+";Set Gruppe");}
        return true;
        

  } else*/

  /* else if(ta=="link") //set parameter in program
  {
      tb = split(input,1); // gruppe int
      uint8_t gp = tb.toInt();
      if(gp < 1 || gp > maxgroups){udpprint("Group out of Range."); return false;}
      link(gp);
      return true;
  }else

  if(ta=="unlink") //set parameter in program
  {
      tb = split(input,1); // gruppe int
      uint8_t gp = tb.toInt();
      if(gp < 1 || gp > maxgroups){udpprint("Group out of Range."); return false;}
      unlink(gp);
      return true;
  }else{
  udpprint("Unknown command!");
  return false;
  }*/
  /*
  if(ta=="savetime") //set parameter in program
  {
      tb = split(input,1); // scene int
      uint8_t sc = tb.toInt();
      if(sc < 1 || sc > MAX_SCENES){udpprint(F("Scene out of Range.")); return false;}
      udpprint(F("Got save time command"));
      saveTime(sc);
     return true;
  }else 
   if(ta=="savegroup") //set parameter in program
  {
      tb = split(input,1); // scene int
      tc = split(input,2); // group int
      uint8_t sc = tb.toInt();
      uint8_t gp = tc.toInt();
      if(sc < 1 || sc > MAX_SCENES){udpprint(F("Scene out of Range.")); return false;}
      if(gp < 1 || gp > MAX_GROUPS){udpprint(F("Group out of Range.")); return false;}
      udpprint(F("Got save group command"));
      saveGroup(sc,gp);
     return true;
  }else */


/*     if(ta=="printgroup") //set parameter in program
  {
      tb = split(input,1); // scene int
      tc = split(input,2); // group int
      uint8_t sc = tb.toInt();
      uint8_t gp = tc.toInt();
      if(sc < 1 || sc > MAX_SCENES){udpprint("Scene out of Range."); return false;}
      if(gp < 1 || gp > MAX_GROUPS){udpprint("Group out of Range."); return false;}
      udpprint("Got print group command");
      printGroup(sc,gp);
     return true;
  }else */

  /*     if(ta=="printscene") //set parameter in program
  {
      tb = split(input,1); // scene int
      uint8_t sc = tb.toInt();
      if(sc < 1 || sc > MAX_SCENES){udpprint("Scene out of Range."); return false;}
      udpprint("Got print scene command");
      printScene(sc);
     return true;
  }else */
  /*if(ta=="seti") //change time
  {
      tb = split(input,1); // szene int
      tc = split(input,2); // parameter string <"s_start" "s_stop", "w_start", "w_stop">
      td = split(input,3); // wert h int
      te = split(input,4); // wert min int 
      uint8_t sc = tb.toInt();
      uint8_t h = td.toInt();
      uint8_t m = te.toInt();
      if(sc < 1 || sc > maxscenes){udpprint(F("Scene out of Range.")); return false;}
      if(h < 0 || h > 23){udpprint(F("Wert h out of range!")); return false;}
      if(h < 0 || h > 59){udpprint(F("Wert min out of range!")); return false;}
      udpprint("Got Command "+ta+" Parameter \""+tc+"\" for Szene "+String(sc)+" Wert "+String(h)+":"+String(m));
      if(tc=="s_start"){scenes[sc-1]->sommer.starth=h; scenes[sc-1]->sommer.startmin=m; return true;}
      if(tc=="s_stop"){scenes[sc-1]->sommer.stoph=h; scenes[sc-1]->sommer.stopmin=m; return true;}
      if(tc=="w_start"){scenes[sc-1]->winter.starth=h; scenes[sc-1]->winter.startmin=m; return true;}
      if(tc=="w_stop"){scenes[sc-1]->winter.stoph=h; scenes[sc-1]->winter.stopmin=m; return true;}
      else{udpprint(F("Unknown parameter!")); return false;}
      
  }else */

 /**/
/*
  if(ta=="set") //set parameter in program
  {
      tb = split(input,1); // szene int
      tc = split(input,2); // gruppe int
      td = split(input,3); // parameter  string <on night white color sat bright kelvin>
      te = split(input,4); // wert int
      uint8_t sc = tb.toInt();
      uint8_t gp = tc.toInt();
      uint8_t wt = te.toInt();
      if(sc < 1 || sc > maxscenes){udpprint(F("Scene out of Range.")); return false;}
      if(gp < 1 || gp > maxgroups){udpprint(F("Group out of Range.")); return false;}
      if(wt < 0 || wt > 255){udpprint(F("Wert out of Range.")); return false;}
      udpprint("Got Command "+ta+" Parameter \""+td+"\" for Szene "+String(sc)+" Gruppe "+String(gp)+" Wert "+String(wt));
      if(td=="on"){if(wt != 0 || wt || 1){udpprint(F("Wert out of range!")); return false;}else{scenes[sc-1]->gruppen[gp-1]->on=wt; return true;}}
      if(td=="night"){if(wt != 0 || wt || 1){udpprint(F("Wert out of range!")); return false;}else{scenes[sc-1]->gruppen[gp-1]->night=wt; return true;}}
      if(td=="white"){if(wt != 0 || wt || 1){udpprint(F("Wert out of range!")); return false;}else{scenes[sc-1]->gruppen[gp-1]->white=wt; return true;}}
      if(td=="color"){scenes[sc-1]->gruppen[gp-1]->color=wt; return true;}
      if(td=="sat"){scenes[sc-1]->gruppen[gp-1]->sat=wt; return true;}
      if(td=="bright"){scenes[sc-1]->gruppen[gp-1]->bright=wt; return true;}
      if(td=="kelvin"){scenes[sc-1]->gruppen[gp-1]->kelvin=wt; return true;}
      else{udpprint(F("Unknown parameter!")); return false;}
      
  }else*/

 /*if(cmd == ANFR_SAVE_SCENE) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        if(sc < 1 || sc > maxscenes){udpprint(msg_sceneerror); return false;}
        saveScene(sc);
        String ret = UDP_ERROR+";"+UDP_ERROR_MSG_OK;                 
        udpprint(ret);
        return true;
  } else*/

    /* if(ta=="savescene") //set parameter in program
  {
      tb = split(input,1); // scene int
      uint8_t sc = tb.toInt();
      if(sc < 1 || sc > MAX_SCENES){udpprint(F("Scene out of Range.")); return false;}
      udpprint(F("Got save scene command"));
      saveScene(sc);
     return true;
  }else */
 /*if(cmd == ANFR_SAVE_GROUP) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        gp = split(input,2).toInt();
        if(sc < 1 || sc > maxscenes){udpprint(msg_sceneerror); return false;}
        if(gp < 1 || gp > maxgroups){udpprint(msg_grouperror); return false;}
        saveGroup(sc,gp);
        String ret = UDP_ERROR+";"+UDP_ERROR_MSG_OK;                 
        udpprint(ret);
        return true;
  } else*/

 /*if(cmd == ANFR_SAVE_TIME) //get scene header (time and active)
  {
        sc = split(input,1).toInt(); // szene int
        if(sc < 1 || sc > maxscenes){udpprint(msg_sceneerror); return false;}
        saveTime(sc);
        String ret = UDP_ERROR+";"+UDP_ERROR_MSG_OK;                 
        udpprint(ret);
        return true;
  } else*/
  /*if(ta=="chli"){ // change light
      tb = split(input,1); // gruppe int
      tc = split(input,2); // parameter string <on night white color sat bright kelvin>
      td = split(input,3); // wert int
      uint8_t gp = tb.toInt();
      uint8_t wt = td.toInt();
      if(gp < 0 || gp > maxgroups){udpprint(F("Group out of Range.")); return false;}
      if(wt < 0 || wt > 255){udpprint(F("Wert out of Range.")); return false;}
      udpprint("Got Command "+ta+" Parameter \""+tc+"\" for Gruppe "+String(gp)+" Wert "+String(wt));
      if(tc=="on"){if(wt != 0 || wt || 1){udpprint(F("Wert out of range!")); return false;}else{setLightOn(gp); return true;}} else
      if(tc=="night"){if(wt != 0 || wt || 1){udpprint(F("Wert out of range!")); return false;}else{setNightmode(gp); return true;}}else
      if(tc=="white"){if(wt != 0 || wt || 1){udpprint(F("Wert out of range!")); return false;}else{setWhitemode(gp); return true;}}else
      if(tc=="color"){setColor(gp,wt); return true;}else
      if(tc=="sat"){setSaturation(gp,wt); return true;}else
      if(tc=="bright"){setBrightness(gp,wt); return true;}else
      if(tc=="kelvin"){setKelvin(gp,wt); return true;}else
      {udpprint(F("Unknown parameter!")); return false;}
  } else*/
  /*if(ta=="get"){ // Get group program settinsg
        tb = split(input,1); // szene int
        tc = split(input,2); // gruppe int
        uint8_t sc = tb.toInt();
        uint8_t gp = tc.toInt();  
        if(sc < 1 || sc > maxscenes){udpprint(F("Scene out of Range.")); return false;}
        if(gp < 1 || gp > maxgroups){udpprint(F("Group out of Range.")); return false;}
        udpprint("\nSzene "+String(sc)+" Gruppe "+String(gp));      
        udpprint("Active: "+String(scenes[sc-1]->active)); 
        udpprint("Zeit Sommer: "+String(scenes[sc-1]->sommer.starth)+":"+String(scenes[sc-1]->sommer.startmin)+" - "+String(scenes[sc-1]->sommer.stoph)+":"+String(scenes[sc-1]->sommer.stopmin)); 
        udpprint("Zeit Winter: "+String(scenes[sc-1]->winter.starth)+":"+String(scenes[sc-1]->winter.startmin)+" - "+String(scenes[sc-1]->winter.stoph)+":"+String(scenes[sc-1]->winter.stopmin)); 
        udpprint("Whitemode: "+String(scenes[sc-1]->gruppen[gp-1]->white));
        udpprint("Nightmode: "+String(scenes[sc-1]->gruppen[gp-1]->night));
        udpprint("ON: "+String(scenes[sc-1]->gruppen[gp-1]->on));
        udpprint("Color: "+String(scenes[sc-1]->gruppen[gp-1]->color));
        udpprint("Saturation: "+String(scenes[sc-1]->gruppen[gp-1]->sat));
        udpprint("Brightness: "+String(scenes[sc-1]->gruppen[gp-1]->bright));
        udpprint("Kelvin: "+String(scenes[sc-1]->gruppen[gp-1]->kelvin));
  }  */
    

  
/*
  bool InterUDP::printScene(uint8_t sc){
  if(sc <= 0 || sc > MAX_SCENES){Serial.println("Scene out of range!"); return false;}
    udpprint("Active: "+String(scenes[sc-1]->active));
    udpprint("Sommer: "+String(scenes[sc-1]->sommer.starth)+":"+String(scenes[sc-1]->sommer.startmin)+"-"+String(scenes[sc-1]->sommer.stoph)+":"+String(scenes[sc-1]->sommer.stopmin));
    udpprint("Winter: "+String(scenes[sc-1]->winter.starth)+":"+String(scenes[sc-1]->winter.startmin)+"-"+String(scenes[sc-1]->winter.stoph)+":"+String(scenes[sc-1]->winter.stopmin));
    for(uint8_t i=0; i<MAX_GROUPS; i++)
    {
        printGroup(sc,i+1);
    }
    return true;
}

bool InterUDP::printGroup(uint8_t sc, uint8_t gp){
  if(sc <= 0 || sc > MAX_SCENES){Serial.println("Scene out of range!"); return false;}
  if(gp <= 0 || gp > MAX_GROUPS){Serial.println("Group out of range!"); return false;}
  
  udpprint("ID: "+String(scenes[sc-1]->gruppen[gp-1]->id));
  udpprint("Color: "+String(scenes[sc-1]->gruppen[gp-1]->color));
  udpprint("Bright: "+String(scenes[sc-1]->gruppen[gp-1]->bright));
  udpprint("Kelvin: "+String(scenes[sc-1]->gruppen[gp-1]->kelvin));
  udpprint("Night: "+String(scenes[sc-1]->gruppen[gp-1]->night));
  udpprint("White: "+String(scenes[sc-1]->gruppen[gp-1]->white));
  udpprint("Sat: "+String(scenes[sc-1]->gruppen[gp-1]->sat));
  udpprint("ON: "+String(scenes[sc-1]->gruppen[gp-1]->on));

  return true;
}*/