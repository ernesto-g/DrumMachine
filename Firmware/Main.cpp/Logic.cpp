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
static unsigned char flagSwShiftState;
static unsigned char currentPattern;
static unsigned char selectedInstrument;
static unsigned char instrumentEncoder0;

static void stateMachineModePlaying(void);
static void stateMachineModeWriting(void);
static void stateMachineModeConfiguration(void);

void logic_init(void)
{
    flagForceScreenUpdate=0;
    mode=MODE_WRITING;
    rthm_stop();
    currentPattern = 0;
    selectedInstrument=0;
    instrumentEncoder0=-1;
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

    // shift sw
    if(frontp_getSwState(SW_SHIFT)==FRONT_PANEL_SW_STATE_JUST_PRESSED)
      flagSwShiftState = 1;
    else if(frontp_getSwState(SW_SHIFT)==FRONT_PANEL_SW_STATE_JUST_RELEASED)
      flagSwShiftState = 0;
    //_________

    
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

int logic_getSelectedInstrument(void)
{
    return selectedInstrument;
}


static void stateMachineModePlaying(void)
{
    // if play/write sw is pressed, pass to writing mode 
    if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
    {
        display_showScreen(SCREEN_WRITING);  
        logic_forceUpdateScreen();
        mode = MODE_WRITING;
        rthm_stop(); // optional
        return;
    }
    //__________________________________________________
      
}
static void stateMachineModeWriting(void)
{
    // if play/write sw is pressed, pass to playing mode 
    if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
    {
        display_showScreen(SCREEN_PLAYING);  
        logic_forceUpdateScreen();
        mode = MODE_PLAYING;
        rthm_playPattern(currentPattern);
        return;
    }
    //__________________________________________________

    // instrument selection
    int instrumentEncoder = frontp_getEncoderPosition();
    if(instrumentEncoder>=INSTRUMENTS_LEN)
      frontp_setEncoderPosition(0);
    if(instrumentEncoder<0)
      frontp_setEncoderPosition(INSTRUMENTS_LEN-1);
      
    if(instrumentEncoder!=instrumentEncoder0)
    {
        instrumentEncoder0 = instrumentEncoder;
        selectedInstrument=1;
        logic_forceUpdateScreen();
    }
    //____________________
    
    
}
static void stateMachineModeConfiguration(void)
{
  
}



