#include <Arduino.h>
#include <EEPROM.h>
#include "RythmManager.h"


#define PATTERNS_ADDR_BASE  0
// 32 patterns with 8 instruments: 512bytes

#define PATTERNS_ENDS_ADDR_BASE  512
// 32 patterns: 32 bytes

#define MIDI_CHN_ADDR_BASE  544 

#define FREE_ADDR_BASE  545 // free eeprom starts here


void mem_init(void)
{
  
}


void mem_savePattern(unsigned char patternIndex,unsigned char instrIndex,unsigned short pattern)
{
    unsigned short address = PATTERNS_ADDR_BASE + patternIndex*(INSTRUMENTS_LEN_WITH_ACC*sizeof(unsigned short)) + (instrIndex*sizeof(unsigned short));
    
    EEPROM.update(address, (unsigned char)(pattern&0xFF));
    EEPROM.update(address+1, (unsigned char)((pattern>>8)&0xFF));
}

unsigned short mem_loadPattern(unsigned char patternIndex,unsigned char instrIndex)
{
    unsigned short address = PATTERNS_ADDR_BASE + patternIndex*(INSTRUMENTS_LEN_WITH_ACC*sizeof(unsigned short)) + (instrIndex*sizeof(unsigned short));
    unsigned short val=0;
     
    val= EEPROM.read(address);
    val = val |   (((unsigned short)EEPROM.read(address+1))<<8);
    return val;
}


void mem_savePatternEnd(unsigned char patternIndex,unsigned char value)
{
    unsigned short address = PATTERNS_ENDS_ADDR_BASE + patternIndex;
    if(value>16)
      value=16;
    
    EEPROM.update(address,value);
}

unsigned char mem_loadPatternEnd(unsigned char patternIndex)
{
    unsigned short address = PATTERNS_ENDS_ADDR_BASE + patternIndex;
    
    unsigned char val = EEPROM.read(address);
    if(val>16)
      val=16;
    
    return val;
}
   
unsigned char mem_loadMidiChn(void)
{
    return EEPROM.read(MIDI_CHN_ADDR_BASE);
}

void mem_saveMidiChn(unsigned char val)
{
    EEPROM.update(MIDI_CHN_ADDR_BASE,val);  
}

