#include <Arduino.h>
#include "RythmManager.h"
#include "InstrumentsManager.h"
#include "DisplayManager.h"

#define SEC_TO_TICK(S)  (S*10000)

static unsigned short patterns[PATTERNS_LEN][INSTRUMENTS_LEN];
static int currentTempo;
static unsigned int currentTempoTicks;
static int stepIndex;
static int stepLen;
static int patternIndex;
static int flagNewStepFinished;

static volatile int tempoCounter;
void rthm_tick(void)
{
    if(tempoCounter>0)
      tempoCounter--;
    
}

void rthm_init(void)
{
  int i,j;  
  // Clean patterns
  for(i=0; i<PATTERNS_LEN; i++)
  {
      for(j=0; j<INSTRUMENTS_LEN; j++)
        patterns[i][j]=0x0000;
  }

  rthm_setTempo(75);
  stepLen=16;
  patternIndex=0;
  flagNewStepFinished=0;

  // prueba pattern
  //patterns[0][INSTR_BD]=0xFFFF;
  //patterns[0][INSTR_SD]=0xFFFF;
  //patterns[0][INSTR_CH]=0xFFFF;
  //patterns[0][INSTR_CB]=0xFF55;
  //patterns[0][INSTR_CB]=0xFFFF;
  
  //patterns[0][INSTR_HC]=B10001000<<8 | B10001000; //0xFFFF;
  //patterns[0][INSTR_SD]=B00010000<<8 | B00010000;
  //patterns[0][INSTR_BD]=B01100111<<8 | B01100111;

  patterns[0][INSTR_SD]=B10010001<<8 | B10010001;
}



void rthm_setTempo(int tempo)
{
    currentTempo = tempo;
    currentTempoTicks = (unsigned int)SEC_TO_TICK( (60.0/tempo) );
}
int rthm_getCurrentTempo(void)
{
  return currentTempo;
}


void rthm_incStep(void)
{
      stepIndex++;
      if(stepIndex>=stepLen)
      {
          stepIndex=0;
      }
}
int rthm_getCurrentStep(void)
{
  return stepIndex;
}

void rthm_playCurrentPattern(void)
{
    tempoCounter=0;
    stepIndex=0;
}
void rthm_playPattern(int pattern)
{
    tempoCounter=0;
    stepIndex=0;
    patternIndex = pattern;
}

void rthm_loop(void)
{  
    if(tempoCounter==0)
    {
        tempoCounter = currentTempoTicks;

        // Check what instruments should be played
        int instrumentIndex;
        for(instrumentIndex=0; instrumentIndex<INSTRUMENTS_LEN; instrumentIndex++)
        {
            if( (patterns[patternIndex][instrumentIndex]>>stepIndex)&0x0001==0x0001)
            {
                inst_playInstrument(instrumentIndex); // Play instrument
            }
        }
        rthm_incStep(); // Inc step        

        flagNewStepFinished=1;
    }
}

int rthm_isNewStepFinished(void)
{
    return flagNewStepFinished;
}
void rthm_resetNewStepFinishedFlag(void)
{
    flagNewStepFinished=0;
}

