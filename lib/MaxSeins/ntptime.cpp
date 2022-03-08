#include <ntptime.h>

NTPtime::NTPtime(GlobalStorage *gs)
{
    store = gs;
}

void NTPtime::begin(void){
    
    udp.begin(2390);
    delay(10);
    loadDST();
    makeRequest();
 }

void NTPtime::makeRequest()
{
    ntp_runflag = true; // wird in ntp_client ganz unten wieder ausgeschalten
    sendNTPpacket(timeServerIP);
}

time_t NTPtime::handleManualTime(void)
{
    time_t x = 0;
    store->newtime=false;
    store->newCalcTime = checkWinter(store->newNTPtime);
    x = store->newCalcTime;
    return x;
}

bool NTPtime::getIsWinter(void)
{
    return store->isWinter;
}

void NTPtime::saveDST(void)
{
  if(DST >=4 && DST <=6){
  EEPROM.begin(512);
  EEPROM.write(ADD_DST,DST);
  EEPROM.commit();
  EEPROM.end();
  }
  else{Serial.println(F("DST save failed! Out of Range."));}
}

void NTPtime::loadDST(void)
{
  EEPROM.begin(512);
  DST = EEPROM.read(ADD_DST);
  if(DST < 4 || DST > 6){DST=6; saveDST(); Serial.println(F("DST Initialized!"));}
  //EEPROM.commit();
  EEPROM.end();
}


time_t NTPtime::checkWinter(time_t t)
{
  if(store->shouldResetDST==1){DST=6; saveDST(); store->shouldResetDST=0;}
  Serial.println("Month: "+String(month(t))+" DST: "+String(DST));
if(month(t)> 3 && month(t)<=10 && DST == 6) // Rudimentärer Ansatz um winter grob zu ermitteln
    {
      store->isWinter = false;  
      DST = 4; 
      saveDST();
      Serial.println(F("Grob isWinter: Nein"));
    }
    else
    {
      store->isWinter = true; 
      DST = 5; 
      saveDST();
      Serial.println(F("Grob isWinter: Ja"));
    }

        // weekday 1 = sunday
        if(weekday(t)==1 && month(t) == 10 && day(t) >= 25 && hour(t) >= 3 && DST == 5) // is winter
        {
          //t = t+3600;
          //isWinter= true;
          DST = 4;
          saveDST();
          Serial.println(F("Fein isWinter: Ja"));
        }

        if (weekday(t) == 1 && month(t) == 3 && day(t) >= 25 && hour(t) ==2 && DST==4) // is summer
        {
            //t = t+7200;
            DST = 5;
            //isWinter= false;
            saveDST();
            Serial.println(F("Fein isWinter: Nein"));
        } 
        if(DST == 5){t = t+3600; store->isWinter = true; }else{t = t+7200; store->isWinter = false;}
        Serial.println("DST = "+String(DST));
        return t;
}

 time_t NTPtime::NTP_client(){
     
     if(ntp_runflag==true){
        int cb = udp.parsePacket();
        if (cb) {
        //Serial.print("packet received, length=");
        //Serial.println(cb);
        udp.read(packetBuffer, NTP_PACKET_SIZE); 
        highWord = word(packetBuffer[40], packetBuffer[41]);
        lowWord = word(packetBuffer[42], packetBuffer[43]);
        secsSince1900 = highWord << 16 | lowWord;
        
        Serial.println("\n#### NETTIME ####");
        //Serial.print("Seconds since Jan 1 1900 = ");
        //Serial.println(secsSince1900);
        Serial.print("Unix time = ");
        epoch = secsSince1900 - seventyYears;
        Serial.println(epoch);
        t = epoch;

// https://www.bmdw.gv.at/Themen/Technik-und-Vermessung/Sommerzeit.html
// https://www.instructables.com/The-Arduino-and-Daylight-Saving-Time-Europe/

// Dieser Block wird nur ausgeführt, wenn 1. Start oder wenn genau über die Zeitumstellung läuft
   t = checkWinter(t);
////////////////////////////////////////////

    

////////////////////////////////////////////

    // print the hour, minute and second:
    Serial.print("The time is ");       // UTC is the time at Greenwich Meridian (GMT)

    int stundex = hour(t); //((epoch  % 86400L) / 3600);
    int minutex = minute(t); //(epoch % 3600) / 60;
    int secondx = second(t);//(epoch % 60);
    int weekdayx = weekday(t);
    Serial.print(weekdayx);
    Serial.print(" ");
    Serial.print(stundex); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (minutex < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print(minutex); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if (secondx < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(secondx); // print the second
    Serial.println("##################\n");
    
    //if(hour(t)!=ntp_h && minute(t)!=ntp_m){listen_NTP=false;}
  }
    ntp_runflag = false; // Nach einer guten Message client wieder ausschalten
    return t;
 } // end of runflag
 return 0;
}


void NTPtime::sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}