#include "InstrumentsManager.h"
#include "RythmManager.h"
#include "DisplayManager.h"
#include "Logic.h"
#include "ios.h"
#include "FrontPanel.h"

static volatile unsigned char tickDivider=0;
ISR(TIMER1_COMPA_vect) // timer1 interrupt. systick. 100uS
{
    // 100uS base time
    rthm_tick();
    inst_tick();
    //_______________
    
    // 1ms base time
    tickDivider++;
    if(tickDivider==10) 
    {
        tickDivider=0;
        frontp_tick1Ms();
    }
    //_____________
}


void setup()   {                
  Serial.begin(9600);

  ios_init();
  display_init();
  frontp_init();
  inst_init();
  rthm_init();
  logic_init();
  
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

  display_showScreen(SCREEN_WRITING);

  Serial.print("DRUM MACHINE INIT OK");
}



void loop() 
{
  logic_loop();  
}


