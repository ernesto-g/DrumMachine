#include <Arduino.h>
#include "ios.h"

void ios_init(void)
{
  pinMode(PIN_TRIGGER_BD, OUTPUT);
  pinMode(PIN_TRIGGER_SD, OUTPUT);
  pinMode(PIN_TRIGGER_CH, OUTPUT);
  pinMode(PIN_TRIGGER_OH, OUTPUT);
  pinMode(PIN_TRIGGER_HC_1, OUTPUT);
  pinMode(PIN_TRIGGER_HC_2, OUTPUT);
  pinMode(PIN_TRIGGER_CB_CV, OUTPUT);
  pinMode(PIN_TRIGGER_T0, OUTPUT);

  // configure SW pins as input
  pinMode(PIN_SW_ENTER,INPUT);
  pinMode(PIN_SW_ESC,INPUT);
  pinMode(PIN_SW_SHIFT,INPUT);
  pinMode(PIN_SW_PLAY_WRITE,INPUT);
  // configure SW pins pullups
  digitalWrite(PIN_SW_ENTER,HIGH);
  digitalWrite(PIN_SW_ESC,HIGH);
  digitalWrite(PIN_SW_SHIFT,HIGH);
  digitalWrite(PIN_SW_PLAY_WRITE,HIGH);
   

}


int ios_readSw(int swPin)
{
    return digitalRead(swPin);
}

void ios_setLo(int triggerNumber)
{
    switch(triggerNumber)
    {
      case IOS_TRIGGER_BD: digitalWrite(PIN_TRIGGER_BD,LOW);break;
      case IOS_TRIGGER_SD: digitalWrite(PIN_TRIGGER_SD,LOW);break;
      case IOS_TRIGGER_CH: digitalWrite(PIN_TRIGGER_CH,LOW);break;
      case IOS_TRIGGER_OH: digitalWrite(PIN_TRIGGER_OH,LOW);break;
      case IOS_TRIGGER_HC_1: digitalWrite(PIN_TRIGGER_HC_1,LOW);break;
      case IOS_TRIGGER_HC_2: digitalWrite(PIN_TRIGGER_HC_2,LOW);break;
      case IOS_TRIGGER_CB_CV: digitalWrite(PIN_TRIGGER_CB_CV,LOW);break;
      case IOS_TRIGGER_T0: digitalWrite(PIN_TRIGGER_T0,LOW);break;
    }
}
void ios_setHi(int triggerNumber)
{
    switch(triggerNumber)
    {
      case IOS_TRIGGER_BD: digitalWrite(PIN_TRIGGER_BD,HIGH);break;
      case IOS_TRIGGER_SD: digitalWrite(PIN_TRIGGER_SD,HIGH);break;
      case IOS_TRIGGER_CH: digitalWrite(PIN_TRIGGER_CH,HIGH);break;
      case IOS_TRIGGER_OH: digitalWrite(PIN_TRIGGER_OH,HIGH);break;
      case IOS_TRIGGER_HC_1: digitalWrite(PIN_TRIGGER_HC_1,HIGH);break;
      case IOS_TRIGGER_HC_2: digitalWrite(PIN_TRIGGER_HC_2,HIGH);break;
      case IOS_TRIGGER_CB_CV: digitalWrite(PIN_TRIGGER_CB_CV,HIGH);break;
      case IOS_TRIGGER_T0: digitalWrite(PIN_TRIGGER_T0,HIGH);break;
    } 
}


