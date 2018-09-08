#include "Arduino.h"
#include "InstrumentsManager.h"
#include "RythmManager.h"
#include "DisplayManager.h"
#include "Logic.h"
#include "FrontPanel.h"
#include "MemoryManager.h"
#include "MidiManager.h"

#define MODE_PLAYING        0
#define MODE_WRITING        1
#define MODE_CONFIGURATION  2

#define WRITING_STATE_INIT      0
#define WRITING_STATE_WRITING   1
#define WRITING_STATE_FINISHED  2

#define PLAYING_STATE_IDLE              0
#define PLAYING_STATE_SELECT_NEXT_PATT  1
#define PLAYING_STATE_ADD_NEXT_PATT     2
 
#define TEMPO_MIN 30
#define TEMPO_MAX 350

static unsigned char mode;
static unsigned char writingState;
static unsigned char playingState;
static unsigned char flagSwShiftState;
static unsigned char flagSwShiftState0;
static unsigned char flagForceScreenUpdate;
static unsigned char currentPattern;
static unsigned char selectedInstrument;
static unsigned char instrumentEncoder0;
static unsigned char patternToWrite;
static unsigned char patternEncoder0;
static unsigned char patternToWriteStep;
static unsigned int tempoEncoder0;
static signed char patternForChain;
static signed char patternForChain0;
static unsigned char chnEncoder0;

static void stateMachineModePlaying(void);
static void stateMachineModeWriting(void);
static void stateMachineModeConfiguration(void);

void logic_init(void)
{
    mem_init();
    
    flagForceScreenUpdate=0;
    mode=MODE_WRITING;
    rthm_stop();
    currentPattern = 0;
    selectedInstrument=0;
    instrumentEncoder0=-1;
    patternToWrite=0;
    patternEncoder0=-1;
    patternToWriteStep=0;
    tempoEncoder0=-1;
    chnEncoder0=-1;

    flagSwShiftState0=0;
    flagSwShiftState=0;

    writingState = WRITING_STATE_INIT;
    playingState = PLAYING_STATE_IDLE;

    // set initial tempo
    tempoEncoder0=150;
    frontp_setEncoderPosition(tempoEncoder0);    
    rthm_setTempo(tempoEncoder0);
    //__________________                   

   logic_forceUpdateScreen();
}

void logic_loop(void)
{
    midi_loop();
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

unsigned char logic_getPatternForChain(void)
{
    return patternForChain;
}

static void stateMachineModePlaying(void)
{
    switch(playingState)
    {
        case PLAYING_STATE_IDLE:
        {
            //Change mode to writing
            if(flagSwShiftState==0)
            {
                // if play/write sw is pressed, pass to writing mode  
                if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
                {
                    display_showScreen(SCREEN_WRITING);  
                    logic_forceUpdateScreen();
                    mode = MODE_WRITING;
                    writingState = WRITING_STATE_INIT;
                    rthm_stop();
                    frontp_resetSwState(SW_PLAY_WRITE);
                    return;
                }
            }    
                  
            // TEMPO selecion
            if(flagSwShiftState==0)
            {
                unsigned int tempoEncoder = frontp_getEncoderPosition();
                if(tempoEncoder>TEMPO_MAX){
                    frontp_setEncoderPosition(TEMPO_MAX);
                    tempoEncoder=TEMPO_MAX;
                }
                if(tempoEncoder<TEMPO_MIN){
                    frontp_setEncoderPosition(TEMPO_MIN);
                    tempoEncoder = TEMPO_MIN;
                }      
                if(tempoEncoder!=tempoEncoder0)
                {
                    tempoEncoder0 = tempoEncoder;
                    rthm_setTempo(tempoEncoder);
                    logic_forceUpdateScreen();
                }
            }
            //_____________________________________________

            // Enter Chain screen
            if(flagSwShiftState==1)
            {
                if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
                {
                    playingState = PLAYING_STATE_SELECT_NEXT_PATT;
                    patternForChain=0;
                    patternForChain0=-1;
                    frontp_setEncoderPosition(patternForChain);   
                    display_showScreen(SCREEN_CHAIN); 
                    frontp_resetSwState(SW_PLAY_WRITE);                
                }
            }
            //_____________
           
            break;
        }
        case PLAYING_STATE_SELECT_NEXT_PATT:
        {
            // Next pattern selecion
            patternForChain = (signed char)frontp_getEncoderPosition();
            if(patternForChain>=PATTERNS_LEN){
                frontp_setEncoderPosition(PATTERNS_LEN-1);
                patternForChain=PATTERNS_LEN-1;
            }
            if(patternForChain<0){
                frontp_setEncoderPosition(0);
                patternForChain = 0;
            }      
            if(patternForChain!=patternForChain0)
            {
                patternForChain0 = patternForChain;
                logic_forceUpdateScreen();
            }
            //_____________________________________________

            // Add current selected pattern in chain
            if(frontp_getSwState(SW_ENTER)==FRONT_PANEL_SW_STATE_SHORT)
            {
                rthm_addPatternToChain(patternForChain);
                logic_forceUpdateScreen();              
                frontp_resetSwState(SW_ENTER);              
            }

            // Quit chain screen
            if(flagSwShiftState==1)
            {
                if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
                {
                    playingState = PLAYING_STATE_IDLE;
                    display_showScreen(SCREEN_PLAYING);
                    frontp_setEncoderPosition(tempoEncoder0);                                       
                    frontp_resetSwState(SW_PLAY_WRITE);
                }   
            }         
            //_____________

            // Remove last pattern in chain
            if(flagSwShiftState==1)
            {
                if(frontp_getSwState(SW_ESC)==FRONT_PANEL_SW_STATE_SHORT)
                {
                    rthm_removeLastPatternInChain();    
                    logic_forceUpdateScreen();       
                    frontp_resetSwState(SW_ESC);     
                }     
            }
            //_____________
            
            break;
        }
    }  
}
static void stateMachineModeWriting(void)
{
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

     
    if(flagSwShiftState==0)
    {
        // if play/write sw was pressed, pass to playing mode
        if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
        {
            //Serial.println("MODO W. Presione playw. currentPattern:");
            currentPattern = patternToWrite; // start playing from last pattern written
            //Serial.println(currentPattern);
            display_showScreen(SCREEN_PLAYING);  
            logic_forceUpdateScreen();
            mode = MODE_PLAYING;
            playingState = PLAYING_STATE_IDLE;
            rthm_playPattern(currentPattern);
            frontp_setEncoderPosition(tempoEncoder0); 
            frontp_resetSwState(SW_PLAY_WRITE);
            return;
        }
        if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_LONG)
        {
            display_showScreen(SCREEN_CONFIG);  
            logic_forceUpdateScreen();
            mode = MODE_CONFIGURATION;
            frontp_resetSwState(SW_PLAY_WRITE);
            frontp_setEncoderPosition(midi_getMidiChn());
            return;          
        }
        
    }
    //__________________________________________________
    
    // instrument selection (shift=false)
    if(flagSwShiftState==0)
    {
        int instrumentEncoder = frontp_getEncoderPosition();
        if(instrumentEncoder>=INSTRUMENTS_LEN_WITH_ACC){
          frontp_setEncoderPosition(0);
          instrumentEncoder=0;
        }
        if(instrumentEncoder<0){
          frontp_setEncoderPosition(INSTRUMENTS_LEN_WITH_ACC-1);
          instrumentEncoder = INSTRUMENTS_LEN_WITH_ACC-1;
        }      
        if(instrumentEncoder!=instrumentEncoder0)
        {
            instrumentEncoder0 = instrumentEncoder;
            selectedInstrument=instrumentEncoder;
            logic_forceUpdateScreen();
            writingState = WRITING_STATE_INIT;
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
            writingState = WRITING_STATE_WRITING;
            break;
        }
        case WRITING_STATE_WRITING:
        {
            if(frontp_getSwState(SW_ENTER)==FRONT_PANEL_SW_STATE_SHORT)
            {
                if(flagSwShiftState==0)
                {
                    rthm_writeSound(patternToWrite,patternToWriteStep,selectedInstrument);
                    inst_playInstrument(selectedInstrument);
                    patternToWriteStep++;
                    if(patternToWriteStep>=rthm_getEndOfPattern(patternToWrite)) {
                        mem_savePattern(patternToWrite,selectedInstrument,rthm_getPattern(patternToWrite,selectedInstrument)); // save into eeprom
                        writingState = WRITING_STATE_FINISHED;
                    } 
                }
                else
                {
                    // go back 1 step
                    if(patternToWriteStep>0)
                      patternToWriteStep--;                
                }
                frontp_resetSwState(SW_ENTER);
                logic_forceUpdateScreen();
            }
            
            if(frontp_getSwState(SW_ESC)==FRONT_PANEL_SW_STATE_SHORT)
            {
                if(flagSwShiftState==0)
                {
                    rthm_writeSilence(patternToWrite,patternToWriteStep,selectedInstrument);
                    patternToWriteStep++;
                    if(patternToWriteStep>=rthm_getEndOfPattern(patternToWrite)) {
                        mem_savePattern(patternToWrite,selectedInstrument,rthm_getPattern(patternToWrite,selectedInstrument)); // save into eeprom
                        writingState = WRITING_STATE_FINISHED; 
                    }
                }
                else
                {
                    rthm_setEndOfPattern(patternToWrite,16);
                    mem_savePatternEnd(patternToWrite,16);
                    rthm_cleanPattern(patternToWrite); // this function saves patterns in eeprom
                    writingState = WRITING_STATE_INIT;
                }
                frontp_resetSwState(SW_ESC);
                logic_forceUpdateScreen();
            } 

           if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
           {
                if(flagSwShiftState==1) // END OF PATTERN WAS PRESSED (shift+play/write button)
                {
                    rthm_setEndOfPattern(patternToWrite,patternToWriteStep);  
                    mem_savePatternEnd(patternToWrite,patternToWriteStep);
                    mem_savePattern(patternToWrite,selectedInstrument,rthm_getPattern(patternToWrite,selectedInstrument)); // save into eeprom                
                    writingState = WRITING_STATE_FINISHED;
                }
                frontp_resetSwState(SW_PLAY_WRITE);
           }
            break;
        }
        case WRITING_STATE_FINISHED:
        {
            
            break;
        }
    }
}
static void stateMachineModeConfiguration(void)
{
    int chnEncoder = frontp_getEncoderPosition();
    if(chnEncoder>255){
      frontp_setEncoderPosition(0);
      chnEncoder=0;
    }
    if(chnEncoder<0){
      frontp_setEncoderPosition(255);
      chnEncoder = 255;
    }      
    if(chnEncoder!=chnEncoder0)
    {
        chnEncoder0 = chnEncoder;
        midi_setMidiChn(chnEncoder);
        logic_forceUpdateScreen();
    }

    // if play/write sw is pressed, pass to writing mode  
    if(frontp_getSwState(SW_PLAY_WRITE)==FRONT_PANEL_SW_STATE_SHORT)
    {
        display_showScreen(SCREEN_WRITING);  
        logic_forceUpdateScreen();
        mode = MODE_WRITING;
        writingState = WRITING_STATE_INIT;
        rthm_stop();
        frontp_resetSwState(SW_PLAY_WRITE);
        mem_saveMidiChn(midi_getMidiChn());
        return;
    }
}



