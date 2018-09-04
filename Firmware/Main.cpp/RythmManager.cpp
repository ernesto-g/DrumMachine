#include <Arduino.h>
#include "RythmManager.h"
#include "InstrumentsManager.h"
#include "DisplayManager.h"
#include "MemoryManager.h"

#define SEC_TO_TICK(S)  (S*10000)


static unsigned short patterns[PATTERNS_LEN][INSTRUMENTS_LEN_WITH_ACC];
static unsigned char patternsEndStep[PATTERNS_LEN];
static int currentTempo;
static unsigned int currentTempoTicks;
static unsigned char stepIndex;
static unsigned char patternIndex;
static unsigned char flagNewStepFinished;
static unsigned char flagPlay;
static signed char patternsChain[PATTERNS_CHAIN_LEN];
static signed char patternChainIndex;

static void loadNextPatternInChain(void);
static unsigned char getChainLen(void);


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
      for(j=0; j<INSTRUMENTS_LEN_WITH_ACC; j++)
      {
          patterns[i][j] = mem_loadPattern(i,j); // load pattern from eeprom          
      }
      patternsEndStep[i]=mem_loadPatternEnd(i); // load patern end from eeprom
      //Serial.print("pattern:");Serial.print(i);Serial.print("END:");Serial.print(patternsEndStep[i]);Serial.print("\n"); 
  }

  for(i=0; i<PATTERNS_CHAIN_LEN; i++)
  {
      patternsChain[i]=-1;
  }
  patternsChain[0]=0; //starts with pattern 0
  
  rthm_setTempo(75); 
  flagNewStepFinished=0;
 
  patternChainIndex=-1;
  loadNextPatternInChain();

  flagPlay=0;
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
      if(stepIndex>=patternsEndStep[patternIndex])
      {
          stepIndex=0;
          // end of pattern, load next pattern
          loadNextPatternInChain();
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
    flagPlay=1;
}
void rthm_playPattern(int pattern)
{
    tempoCounter=0;
    stepIndex=0;
    patternIndex = pattern;
    flagPlay=1;

    if(getChainLen()==1)
    {
        patternsChain[0]=patternIndex;
        patternChainIndex=0;
    }
      
}
int rthm_getCurrentPattern(void)
{
  return patternIndex;
}


void rthm_stop(void)
{
    flagPlay=0;
}

unsigned short rthm_getPattern(unsigned char patIndex,unsigned char instrIndex)
{
  return patterns[patIndex][instrIndex];  
}

void rthm_writeSound(unsigned char patIndex,unsigned char patternToWriteStep,unsigned char instrIndex)
{
    patterns[patIndex][instrIndex]|= (1<<patternToWriteStep);
}
void rthm_writeSilence(unsigned char patIndex,unsigned char patternToWriteStep,unsigned char instrIndex)
{
    patterns[patIndex][instrIndex]&= ~((unsigned short)(1<<patternToWriteStep));
}
void rthm_cleanPattern(unsigned char patIndex)
{
    unsigned char i;
    for(i=0; i<INSTRUMENTS_LEN_WITH_ACC; i++)
    {
      patterns[patIndex][i]=0x0000;  
      mem_savePattern(patIndex,i,0x0000); // save into eeprom
    }
}

void rthm_setEndOfPattern(unsigned char patIndex,unsigned char patIndexMax)
{
    patternsEndStep[patIndex] = patIndexMax;
}

unsigned char rthm_getEndOfPattern(unsigned char patIndex)
{
    return patternsEndStep[patIndex];
}

void rthm_removeLastPatternInChain(void)
{
    if(getChainLen()<=1)
      return;
  
    int i;
    for(i=PATTERNS_CHAIN_LEN-1; i>=0; i--)
    {
        if(patternsChain[i]!=-1)
        {
            patternsChain[i] = -1;
            return;
        }
    }
}
void rthm_addPatternToChain(unsigned char newPattern)
{
    int i;
    for(i=0; i<PATTERNS_CHAIN_LEN; i++)
    {
        if(patternsChain[i]==-1)
        {
            patternsChain[i] = newPattern;
            return;
        }
    }
}
static unsigned char getChainLen(void)
{
    int i;
    for(i=0; i<PATTERNS_CHAIN_LEN; i++)
    {
        if(patternsChain[i]==-1)
        {
            return i;
        }
    }
    return i;
}


signed char* rthm_getPatternsChain(void)
{
    return patternsChain;
}

void rthm_loop(void)
{  
    if(tempoCounter==0 && flagPlay==1)
    {
        tempoCounter = currentTempoTicks;

        // check accent state
        if( (patterns[patternIndex][INSTR_ACC]>>stepIndex)&0x0001==0x0001)
            inst_accOn();
        else
            inst_accOff();
        //___________________

        // Check what instruments should be played
        unsigned char instrumentIndex;
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

static void loadNextPatternInChain(void)
{
    patternChainIndex++;
    if(patternChainIndex>=getChainLen())
      patternChainIndex=0;

    patternIndex=patternsChain[patternChainIndex];             
}

