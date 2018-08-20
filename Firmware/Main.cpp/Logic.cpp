#include "Arduino.h"
#include "InstrumentsManager.h"
#include "RythmManager.h"
#include "DisplayManager.h"
#include "Logic.h"
#include "FrontPanel.h"

#define MODE_PLAYING        0
#define MODE_WRITING        1
#define MODE_CONFIGURATION  2

#define WRITING_STATE_INIT  0

static unsigned char flagForceScreenUpdate;
static unsigned char mode;
static unsigned char flagSwShiftState;
static unsigned char flagSwShiftState0;
static unsigned char currentPattern;
static unsigned char selectedInstrument;
static unsigned char instrumentEncoder0;
static unsigned char patternToWrite;
static unsigned char patternEncoder0;
static unsigned char patternToWriteStep;
static unsigned char writingState;


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
    patternToWrite=0;
    patternEncoder0=-1;
    patternToWriteStep=0;

    flagSwShiftState0=0;
    flagSwShiftState=0;

    writingState = WRITING_STATE_INIT;
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
    {
        if(flagSwShiftState!=1)
          logic_forceUpdateScreen();  
        flagSwShiftState = 1;
    }
    else if(frontp_getSwState(SW_SHIFT)==FRONT_PANEL_SW_STATE_JUST_RELEASED)
    {
       if(flagSwShiftState!=0)
          logic_forceUpdateScreen();  
        flagSwShiftState = 0;
    }
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

int logic_getSwShiftState(void)
{
    return flagSwShiftState;
}

int logic_getWritingPattern(void)
{
    return patternToWrite;
}

int logic_getWritingPatternStep(void)
{
    return patternToWriteStep;  
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
        Serial.println("MODO W. Presione playw");
        display_showScreen(SCREEN_PLAYING);  
        logic_forceUpdateScreen();
        mode = MODE_PLAYING;
        rthm_playPattern(currentPattern);
        return;
    }
    //__________________________________________________


    // shift sw changed. set initial value for encoder counter
    if(flagSwShiftState0!=flagSwShiftState)
    {
        flagSwShiftState0=flagSwShiftState;
        if(flagSwShiftState==0)
          frontp_setEncoderPosition(instrumentEncoder0);
        else
          frontp_setEncoderPosition(patternEncoder0);    
    }
    //________________________________________________________

    // instrument selection (shift=false)
    if(flagSwShiftState==0)
    {
        int instrumentEncoder = frontp_getEncoderPosition();
        if(instrumentEncoder>=INSTRUMENTS_LEN){
          frontp_setEncoderPosition(0);
          instrumentEncoder=0;
        }
        if(instrumentEncoder<0){
          frontp_setEncoderPosition(INSTRUMENTS_LEN-1);
          instrumentEncoder = INSTRUMENTS_LEN-1;
        }      
        if(instrumentEncoder!=instrumentEncoder0)
        {
            Serial.println("cambie encoder instrument!");
            Serial.println(selectedInstrument);
            instrumentEncoder0 = instrumentEncoder;
            selectedInstrument=instrumentEncoder;
            logic_forceUpdateScreen();
        }
    }
    //____________________
    
    // next pattern selection (shift=true)
    if(flagSwShiftState==1)
    {
        int patternEncoder = frontp_getEncoderPosition();
        if(patternEncoder>=PATTERNS_LEN){
          frontp_setEncoderPosition(0);
          patternEncoder=0;
        }
        if(patternEncoder<0){
          frontp_setEncoderPosition(PATTERNS_LEN-1);
          patternEncoder = PATTERNS_LEN-1;
        }      
        if(patternEncoder!=patternEncoder0)
        {
            Serial.println("cambie encoder pattern!");
            patternEncoder0 = patternEncoder;
            patternToWrite = patternEncoder;
            writingState = WRITING_STATE_INIT;
            logic_forceUpdateScreen();
        }
    }
    //_____________________________________


    switch(writingState)
    {
        case WRITING_STATE_INIT:
        {
            patternToWriteStep=0;
            break;
        }
    }
}
static void stateMachineModeConfiguration(void)
{
  
}



