#include "InstrumentsManager.h"
#include "RythmManager.h"
#include "DisplayManager.h"
#include "Logic.h"



ISR(TIMER1_COMPA_vect) // timer1 interrupt. systick. 100uS
{
    rthm_tick();
    inst_tick();
}


void setup()   {                
  Serial.begin(9600);

  display_init();

  inst_init();
  rthm_init();

  // initialize timer1 (systick)
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1600;            // compare match register (16MHz/presc)*T = (16MHz/1)*100uS = 1600
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);    // 1 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
  //___________________________

  
  // Prueba pulso secuencer
  /*
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(7,LOW);
  digitalWrite(6,LOW);
  while(1)
  {
    digitalWrite(7,HIGH);
    delay(1);
    digitalWrite(7,LOW);
    delay(9);

    digitalWrite(7,HIGH);
    delay(1);
    digitalWrite(7,LOW);
    delay(9);

    digitalWrite(7,HIGH);
    delay(1);
    digitalWrite(7,LOW);
    delay(9);

   
    digitalWrite(6,HIGH);
    delay(1);
    digitalWrite(6,LOW);
    
    delay(1000);
  }
  */


  display_showMainScreen();
}



void loop() 
{
  logic_loop();


    
}


