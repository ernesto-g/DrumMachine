#include <Arduino.h>
#include "RythmManager.h"
#include "InstrumentsManager.h"
#include "ios.h"

#define STATE_IDLE      		0
#define STATE_START     		1
#define STATE_WAITING   		2
#define STATE_FINISHED  		3
#define STATE_WAIT_HANDCLAP_SM	4

#define STATE_HC_IDLE	0
#define STATE_HC_START	1
#define STATE_HC_WAIT_PULSE1_HI	2
#define STATE_HC_WAIT_PULSE1_LO	3
#define STATE_HC_WAIT_PULSE2_HI	4
#define STATE_HC_WAIT_PULSE2_LO	5
#define STATE_HC_WAIT_PULSE3_HI	6
#define STATE_HC_WAIT_PULSE3_LO	7
#define STATE_HC_WAIT_PULSE4_HI	8

#define TRIGGER_PULSE_1MS	10
#define TRIGGER_PULSE_9MS 90


static unsigned char states[INSTRUMENTS_LEN];
static volatile char timeouts[INSTRUMENTS_LEN];
static unsigned char stateHandClap;

static void stateMachine(int instrumentIndex);
static int getTimeoutByInstrument(int instrumentIndex);
static void handClapStart(void);
static int stateMachineHandClap(void);
static int getTriggerPinByInstrument(int instrumentIndex);


void inst_tick(void)
{
	int i;
	for(i=0; i<INSTRUMENTS_LEN; i++)
	{
		if(timeouts[i]>0)
			timeouts[i]--;
	}
}

void inst_init(void)
{
	int i;
	for(i=0; i<INSTRUMENTS_LEN; i++)
	{
		timeouts[i]=0;
		states[i]=STATE_IDLE;
	} 
  
  stateHandClap = STATE_HC_IDLE;
  
}

void inst_loop(void)
{
	int i;
	for(i=0; i<INSTRUMENTS_LEN; i++)
	{
		stateMachine(i);
	}
}

void inst_playInstrument(int instrumentIndex)
{
    states[instrumentIndex] = STATE_START;
}

int inst_areAllInstrumentsIdle(void)
{
    int i;
    for(i=0; i<INSTRUMENTS_LEN; i++)
    {
        if(states[i]!=STATE_IDLE)
          return 0;
    }
    return 1;
}


static void stateMachine(int instrumentIndex)
{
    switch(states[instrumentIndex])
    {
        case STATE_IDLE:
        {
            break;
        }
        case STATE_START:
        {
      			if(instrumentIndex==INSTR_HC)
      			{
      				handClapStart();
      				states[instrumentIndex] = STATE_WAIT_HANDCLAP_SM;
      			}
      			else
      			{
      				ios_setHi(getTriggerPinByInstrument(instrumentIndex));
      				timeouts[instrumentIndex] = getTimeoutByInstrument(instrumentIndex);
      				states[instrumentIndex] = STATE_WAITING;
      			}
            break;
        }
        case STATE_WAITING:
        {
          if(timeouts[instrumentIndex]<=0)
          {
            ios_setLo(getTriggerPinByInstrument(instrumentIndex));
            states[instrumentIndex] = STATE_FINISHED;
          }
          break;
        }
        case STATE_FINISHED:
        {
          states[instrumentIndex] = STATE_IDLE;
          break;
        }
    		case STATE_WAIT_HANDCLAP_SM:
    		{
          //Serial.println("ejecuto st hc");
    			if(stateMachineHandClap()==1)
    				states[instrumentIndex] = STATE_IDLE;
    			break;
    		}
    }
}



static void handClapStart(void)
{
	stateHandClap=STATE_HC_START;
}

static int stateMachineHandClap(void)
{
	int ret=0;
	switch(stateHandClap)
	{
		case STATE_HC_IDLE:
		{
			break;
		}
		case STATE_HC_START:
		{
			ios_setHi(IOS_TRIGGER_HC_1);
			timeouts[INSTR_HC] = TRIGGER_PULSE_1MS;
			stateHandClap = STATE_HC_WAIT_PULSE1_HI;
			break;
		}
		case STATE_HC_WAIT_PULSE1_HI:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setLo(IOS_TRIGGER_HC_1);
				timeouts[INSTR_HC] = TRIGGER_PULSE_9MS;
				stateHandClap = STATE_HC_WAIT_PULSE1_LO;
			}
			break;
		}
		case STATE_HC_WAIT_PULSE1_LO:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setHi(IOS_TRIGGER_HC_1);
				timeouts[INSTR_HC] = TRIGGER_PULSE_1MS;
				stateHandClap = STATE_HC_WAIT_PULSE2_HI;
			}
			break;
		}
		case STATE_HC_WAIT_PULSE2_HI:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setLo(IOS_TRIGGER_HC_1);
				timeouts[INSTR_HC] = TRIGGER_PULSE_9MS;
				stateHandClap = STATE_HC_WAIT_PULSE2_LO;
			}
			break;
		}
		case STATE_HC_WAIT_PULSE2_LO:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setHi(IOS_TRIGGER_HC_1);
				timeouts[INSTR_HC] = TRIGGER_PULSE_1MS;
				stateHandClap = STATE_HC_WAIT_PULSE3_HI;
			}
			break;
		}	
		case STATE_HC_WAIT_PULSE3_HI:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setLo(IOS_TRIGGER_HC_1);
				timeouts[INSTR_HC] = TRIGGER_PULSE_9MS;
				stateHandClap = STATE_HC_WAIT_PULSE3_LO;
			}
			break;
		}	
		case STATE_HC_WAIT_PULSE3_LO:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setHi(IOS_TRIGGER_HC_2);
				timeouts[INSTR_HC] = TRIGGER_PULSE_1MS;
				stateHandClap = STATE_HC_WAIT_PULSE4_HI;
			}
			break;
		}
		case STATE_HC_WAIT_PULSE4_HI:
		{
			if(timeouts[INSTR_HC]<=0)
			{
				ios_setLo(IOS_TRIGGER_HC_2);
				stateHandClap = STATE_HC_IDLE;
				ret=1;
			}
			break;
		}		
	}
	return ret;
}

static int getTriggerPinByInstrument(int instrumentIndex)
{
  switch(instrumentIndex)
  {
    case INSTR_BD:  return IOS_TRIGGER_BD;
    case INSTR_SD:  return IOS_TRIGGER_SD;
    case INSTR_CH:  return IOS_TRIGGER_CH;
    case INSTR_OH:  return IOS_TRIGGER_OH;
    //case INSTR_HC:  return ;
    case INSTR_CB:  return IOS_TRIGGER_CB_CV;
    case INSTR_CV:  return IOS_TRIGGER_CB_CV;
    case INSTR_TO:  return IOS_TRIGGER_T0;   
  }  
  return -1;  
}
static int getTimeoutByInstrument(int instrumentIndex)
{
	switch(instrumentIndex)
	{
		case INSTR_BD:	return TRIGGER_PULSE_1MS;
		case INSTR_SD:	return TRIGGER_PULSE_1MS;
		case INSTR_CH:	return TRIGGER_PULSE_1MS;
		case INSTR_OH:	return TRIGGER_PULSE_1MS;
		//case INSTR_HC:	return TRIGGER_PULSE_1MS;
		case INSTR_CB:	return TRIGGER_PULSE_1MS;
		case INSTR_CV:	return TRIGGER_PULSE_1MS;
		case INSTR_TO:	return TRIGGER_PULSE_1MS;		
	}
	return -1;
}
