
#include "buttons.h"


ButtonsX::ButtonsX(GlobalStorage *gs)
{
store = gs;
}

ButtonsX::~ButtonsX()
{
  scene_mode=MODE_OFF;
}

void ButtonsX::checkLeds(bool manual = false){

uint8_t sc = 0;
if(manual==true){sc = store->SELECTED_SCENE;}else{sc = store->ACTIVE_SCENE;}
    if(store->SCENE_MODE == MODE_OFF){sc = 0;}
    switch(sc)
    {
        case 0: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,LOW); break;
        case 1: mcp.digitalWrite(LED_SC1,HIGH); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,LOW); break; 
        case 2: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,HIGH); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,LOW); break;
        case 3: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,HIGH); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,LOW); break;
        case 4: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,HIGH); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,LOW); break;
        case 5: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,HIGH);  mcp.digitalWrite(LED_SC6,LOW); break;
        case 6: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,HIGH); break;
        default: mcp.digitalWrite(LED_SC1,LOW); mcp.digitalWrite(LED_SC2,LOW); mcp.digitalWrite(LED_SC3,LOW); mcp.digitalWrite(LED_SC4,LOW); mcp.digitalWrite(LED_SC5,LOW);  mcp.digitalWrite(LED_SC6,LOW); break;
    }
    switch(store->SCENE_MODE)
    {
        case MODE_AUTO: mcp.digitalWrite(LED_AUTO,HIGH); break;
        case MODE_MANUAL: if(store->should_load==0 && store->busy==false){mcp.digitalWrite(LED_AUTO,LOW);} break;
        default: break;
    }


}


void ButtonsX::wlanstatus(bool ok)
{
    if(ok==false){
        mcp.digitalWrite(LED_SC6,HIGH);
    }
    else{
      mcp.digitalWrite(LED_SC6,LOW);
    }

}
void ButtonsX::timestatus(bool ok)
{
    if(ok==false){
        mcp.digitalWrite(LED_TIME,HIGH);
        Serial.println(F("Timeled High"));
    }
    else{
      mcp.digitalWrite(LED_TIME,LOW);
      Serial.println(F("Timeled Low"));
    }

}
void ButtonsX::autoblink(void)
{

unsigned long curr = millis();
  if(store->SCENE_MODE == MODE_OFF && store->busy==false){
      if(curr-pre_blink >= inter_blink)
      {
        pre_blink = curr;
        if(mcp.digitalRead(LED_AUTO)==LOW){mcp.digitalWrite(LED_AUTO,HIGH); inter_blink = 125;} else {mcp.digitalWrite(LED_AUTO,LOW); inter_blink = 5000;}
      } 
  }

if(store->SCENE_MODE==MODE_MANUAL && store->should_load==1 && store->busy == false)
{
  if(curr-pre_blink >= inter_blink)
      {
        pre_blink = curr;
        inter_blink = 150;
        mcp.digitalWrite(LED_AUTO, !mcp.digitalRead(LED_AUTO));
      }
}
  
}

//void Buttons::AutoLow(void){mcp.digitalWrite(LED_AUTO,LOW);}

//void Buttons::AutoHigh(void){mcp.digitalWrite(LED_AUTO,HIGH);}

void ButtonsX::AutoBusy(bool bu){
    if(bu==true){
        store->busy=true; 
        mcp.digitalWrite(LED_AUTO,HIGH); 
    }else{
      store->busy=false;
      mcp.digitalWrite(LED_AUTO,LOW);
    } 
}


void ButtonsX::debounce(void)
{
    unsigned long a = millis();
//uint8_t scene = 1;
//uint8_t autos = 1;

    active_scene = store->ACTIVE_SCENE;
    scene_mode = store->SCENE_MODE;
    selected_scene = store->SELECTED_SCENE;

    scene = mcp.digitalRead(BUT_SCENE);
    autos = mcp.digitalRead(BUT_AUTO);
    #ifdef DEBUG_BUTTONS
      //Serial.println("Scene But: "+String(scene)+" Auto But: "+String(autos));
    #endif

if(state_scene==false && scene == LOW)
  {     
        scene_mode = MODE_MANUAL;
        store->SCENE_MODE = MODE_MANUAL;
        pre_scene = a;
        state_scene = true;
        if(selected_scene < MAX_SCENES){
            selected_scene++;
        }
        else{
            selected_scene = 1;
        }
        store->should_load = 1;
        store->SELECTED_SCENE = selected_scene;
        checkLeds(true);
        //Serial.println("Should Load(1) = "+String(store->should_load));
        #ifdef DEBUG_BUTTONS
          Serial.println("BUT SCENE "+String(selected_scene));
        #endif
    } 
    if(state_scene == true && a-pre_scene >= inter)
    {
      state_scene = false;
    } 


if(state_auto==false && autos == LOW)
  {
    state_auto = true;
    pre_auto = a;

  if(store->should_load==1){store->should_load=2; /*Serial.println("Should Load(2) = "+String(store->should_load));*/ /*AutoBusy(true);*/ }

  if(store->should_load==0){
    if(scene_mode == MODE_OFF){
        scene_mode=MODE_AUTO;
        store->SCENE_MODE = MODE_AUTO;
        mcp.digitalWrite(LED_AUTO,HIGH);
        checkLeds();
        #ifdef DEBUG_BUTTONS
            Serial.println(F("AUTO MODE"));
        #endif
    }else      
    {
        scene_mode=MODE_OFF;
        store->SCENE_MODE = MODE_OFF;
        mcp.digitalWrite(LED_AUTO,LOW);
        checkLeds();
        #ifdef DEBUG_BUTTONS
            Serial.println(F("OFF MODE"));
        #endif
    }
  }
    } 
    if(state_auto == true && a-pre_auto >= inter)
    {
      state_auto = false;
    } 
      //store->SCENE_MODE = scene_mode;
      //store->ACTIVE_SCENE = active_scene;
      
      
}




void ButtonsX::begin(void){
    
    mcp.begin(); // 0x20
    mcp.pinMode(BUT_AUTO,INPUT); //External Pullups
    mcp.pinMode(BUT_SCENE,INPUT); //External Pullups
    
    
    mcp.pinMode(LED_SC1,OUTPUT);
    mcp.pinMode(LED_SC2,OUTPUT);
    mcp.pinMode(LED_SC3,OUTPUT);
    mcp.pinMode(LED_SC4,OUTPUT);
    mcp.pinMode(LED_SC5,OUTPUT);
    mcp.pinMode(LED_SC6,OUTPUT);
    mcp.pinMode(LED_AUTO,OUTPUT);
    mcp.pinMode(LED_TIME,OUTPUT);

    mcp.digitalWrite(LED_SC1,LOW);
    mcp.digitalWrite(LED_SC2,LOW);
    mcp.digitalWrite(LED_SC3,LOW);
    mcp.digitalWrite(LED_SC4,LOW);
    mcp.digitalWrite(LED_SC5,LOW);
    mcp.digitalWrite(LED_SC6,LOW);
    mcp.digitalWrite(LED_AUTO,LOW);

    delay(5);
    if(scanI2C()<1)
    {
            Serial.println(F("No I2C Devide (Buttons+LEDs) gefunden!"));
            delay(1000);
            ESP.restart();
    }

}

uint8_t ButtonsX::scanI2C(void)
{
    byte error, address;
    uint8_t nDevices;
 
  Serial.println(F("Scanning..."));
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print(F("Unknown error at address 0x"));
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found\n"));
  else
    Serial.println(F("done\n"));
  return nDevices;
  }

  /*void Buttons::checkButtons(uint8_t *ari){

scene_mode = ari[1];
active_scene = ari[0];
//b_auto.update();
//b_scene.update();
//int autos = b_auto.read();
//int scene = b_scene.read();
//autos = digitalRead(BUT_AUTO);
//scene = digitalRead(BUT_SCENE);

//Serial.println("AUto: "+String(autos)+" Scene: "+String(scene));

  if ( autos == 0 ) {
    if(scene_mode == MODE_OFF){
        scene_mode=MODE_AUTO;
        mcp.digitalWrite(LED_AUTO,HIGH);
        #ifdef DEBUG_BUTTONS
            Serial.println(F("Button AUTO pressed"));
        #endif
    }
    else{
        scene_mode=MODE_OFF;
        mcp.digitalWrite(LED_AUTO,LOW);
        #ifdef DEBUG_BUTTONS
            Serial.println(F("Button MANUAL pressed"));
        #endif
    }
  } 

  if ( scene == 0) {
    if(active_scene < max_scenes){
        active_scene++;
    }
    else{
        active_scene = 1;
    }
        #ifdef DEBUG_BUTTONS
            Serial.println("Button NEXT SCENE pressed");
        #endif
    checkLeds();
  } 
  
  ari[0] = scene_mode;
  ari[1] = active_scene;

}*/