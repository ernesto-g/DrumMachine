#include <Arduino.h>
#include "MidiManager.h"
#include "InstrumentsManager.h"
#include "DisplayManager.h"
#include "MemoryManager.h"


MidiInfo midiInfo;
unsigned char midiStateMachine=MIDI_STATE_IDLE;
unsigned char voicesMode;

static void midi_analizeMidiInfo(MidiInfo * pMidiInfo);
static unsigned char midiChannel;
static volatile unsigned char timeout;

void midi_init(void)
{
    midiChannel = mem_loadMidiChn();
    timeout=0;
}

void midi_tickMs(void)
{
    if(timeout>0)
      timeout--;
}

void midi_setMidiChn(unsigned char val)
{
    midiChannel = val;
}

unsigned char midi_getMidiChn(void)
{
    return midiChannel;
}

void midi_stateMachine(unsigned char midiByte)
{
  timeout=MIDI_BYTE_TIMEOUT;
  
  switch(midiStateMachine)
  {
      case MIDI_STATE_IDLE:
        {
        // read status byte 
        midiInfo.channel = midiByte & B00001111;
        midiInfo.cmd = midiByte & B11110000;
        midiStateMachine = MIDI_STATE_RVC_DATA1;
        break;
      }
      case MIDI_STATE_RVC_DATA1:
      {
        // read note
        midiInfo.note = midiByte;
        midiStateMachine = MIDI_STATE_RVC_DATA2;        
        break;
      }
      case MIDI_STATE_RVC_DATA2:
      {
        // read velocity
        midiInfo.vel = midiByte;
        midiStateMachine = MIDI_STATE_IDLE;
        midi_analizeMidiInfo(&midiInfo);
        break;
      }
  }

    
}

void midi_loop(void)
{

  if(timeout==0)
      midiStateMachine = MIDI_STATE_IDLE;  
}


static void midi_analizeMidiInfo(MidiInfo * pMidiInfo)
{
    if(pMidiInfo->channel==midiChannel)
    {
        if(pMidiInfo->cmd==MIDI_CMD_NOTE_ON)
        {
            // NOTE ON 
            switch(pMidiInfo->note)
            {
                case 33:
                case 35:
                case 36:
                  inst_playInstrument(INSTR_BD);
                  break;
                case 31:
                case 38:
                case 40:
                  inst_playInstrument(INSTR_SD);
                  break;
                case 39:
                case 54:
                case 69:
                case 70:
                case 82:
                  inst_playInstrument(INSTR_CP);
                  break;
                case 44:
                case 46:
                case 53:
                  inst_playInstrument(INSTR_OH);
                  break;
                case 42:
                  inst_playInstrument(INSTR_CH);
                  break;
                case 32:
                case 34:
                case 37:
                case 75:
                  inst_playInstrument(INSTR_CL);
                  break;
                case 48:
                  inst_playInstrument(INSTR_TO);
                  break;                
            }
        }
        else if(pMidiInfo->cmd==MIDI_CMD_NOTE_OFF)
        {
            // NOTE OFF
        }
        else if(pMidiInfo->cmd==MIDI_CMD_CONTROL_CHANGE)
        {
          // control change
        }
        else
        {
          // unsuported command          
        }
    }     
}

