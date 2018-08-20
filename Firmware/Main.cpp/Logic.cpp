#include "Arduino.h"
#include "InstrumentsManager.h"
#include "RythmManager.h"
#include "DisplayManager.h"
#include "Logic.h"
#include "FrontPanel.h"

#define MODE_PLAYING        0
#define MODE_WRITING        1
#define MODE_CONFIGURATION  2

static unsigned char flagForceScreenUpdate;
static unsigned char mode;


static void stateMachineModePlaying(void);
static void stateMachineModeWriting(void);
static void stateMachineModeConfiguration(void);


void logic_init(void)
{
    flagForceScreenUpdate=0;
    mode=MODE_WRITING;
    rthm_stop();
}

void logic_loop(void)
{
    frontp_loop();
    rthm_loop();
    inst_loop();
    display_loop();

    // Update screen after all instruments are finished
    if(rthm_isNewStepFinished()==1 || flagForceScreenUpdate==1)
    {  
      // Instruments just started to be played
      // wait until they finish
      if(inst_areAllInstrumentsIdle())
      {
            display_update(); // once all instruments are idle, refresh the screen
            rthm_resetNewStepFinishedFlag();
            flagForceScreenUpdate=0;
      }
    }
    //_________________________________________________

    /*
    // test sw
    int state = frontp_getSwState(SW_ENTER);
    switch(state)
    {
        case FRONT_PANEL_SW_STATE_JUST_PRESSED:
        Serial.println("JP");
        frontp_resetSwState(SW_ENTER);
        break;
        case FRONT_PANEL_SW_STATE_SHORT:
        Serial.println("pulsada corta");
        frontp_resetSwState(SW_ENTER);
        break;
        case FRONT_PANEL_SW_STATE_LONG:
        Serial.println("pulsada larga");
        frontp_resetSwState(SW_ENTER);
        break;
        case FRONT_PANEL_SW_STATE_JUST_RELEASED:
        Serial.println("JR");
        frontp_resetSwState(SW_ENTER);
        break;
    }*/


    switch(mode)
    {
        case MODE_PLAYING:
        {
            stateMachineModePlaying();
            break;
        }
        case MODE_WRITING:
        {
            stateMachineModeWriting();
            break;
        }
        case MODE_CONFIGURATION:
        {
            stateMachineModeConfiguration();
            break;
        }
    }
   
}

void logic_forceUpdateScreen(void)
{
    flagForceScreenUpdate=1;
}


static void stateMachineModePlaying(void)
{
      
}
static void stateMachineModeWriting(void)
{
  
}
static void stateMachineModeConfiguration(void)
{
  
}



