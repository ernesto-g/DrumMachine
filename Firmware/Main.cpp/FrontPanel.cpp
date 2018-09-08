/**
 *  Arduino Analog Drum Machine
    Copyright (C) <2018>  Ernesto Gigliotti <ernestogigliotti@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <Arduino.h>
#include "RotaryEncoder.h"
#include "FrontPanel.h"
#include "ios.h"

#define LEN_SW          4

#define TIMEOUT_BOUNCE      20 // 10ms
#define TIMEOUT_SHORT_PRESS 1000  // 1sec
#define TIMEOUT_LONG_PRESS  2000  // 2sec

#define STATE_IDLE                  0
#define STATE_PRESSED               1
#define STATE_WAIT_BOUNCE           2
#define STATE_PRESS_CONFIRMED       3
#define STATE_WAIT_RELEASE          4
#define STATE_WAIT_RELEASE2         5
#define STATE_WAIT_BOUNCE_RELEASE   6


static RotaryEncoder encoder(A2, A3);
static unsigned char state[LEN_SW];
static unsigned char switchesState[LEN_SW];
static volatile unsigned int timeouts[LEN_SW];

static int getPin(int swIndex);
static void swStateMachine(int swIndex);


void frontp_tick1Ms(void)
{
    unsigned char i;
    for(i=0; i<LEN_SW; i++)
    {
        if(timeouts[i]<=TIMEOUT_LONG_PRESS)
          timeouts[i]++;  
    }
}

// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A2 and A3: exactly where we need to check.
ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.

}

void frontp_init(void)
{
    // Enable interrupts for A2 A3 pins
    PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
    PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.
}

int frontp_getEncoderPosition(void)
{
    return encoder.getPosition();
}

void frontp_setEncoderPosition(int pos)
{
    encoder.setPosition(pos);
}

void frontp_loop(void)
{
    /*
    static int pos = 0;
    int newPos = encoder.getPosition();
    //Serial.println(newPos);
    if (pos != newPos) {
      Serial.print(newPos);
      Serial.println();
      pos = newPos;
    }
    */
    
    int i;
    for(i=0;i<LEN_SW; i++)
      swStateMachine(i);

}

int frontp_getSwState(int swIndex)
{
    return switchesState[swIndex];
}
void frontp_resetSwState(int swIndex)
{
    switchesState[swIndex]=FRONT_PANEL_SW_STATE_IDLE;
}


static int getPin(int swIndex)
{
    switch(swIndex)
    {
        case SW_ENTER:  return PIN_SW_ENTER;
        case SW_ESC:  return PIN_SW_ESC;
        case SW_SHIFT:  return PIN_SW_SHIFT;
        case SW_PLAY_WRITE:  return PIN_SW_PLAY_WRITE;        
    }
    return -1;
}



static void swStateMachine(int swIndex)
{
    switch(state[swIndex])
    {
        case STATE_IDLE:
        {
            if(ios_readSw(getPin(swIndex))==LOW)
            {
                // sw pressed
                state[swIndex] = STATE_PRESSED;
            }
            break; 
        }
        case STATE_PRESSED:
        {
            timeouts[swIndex]=0;
            state[swIndex] = STATE_WAIT_BOUNCE;            
            break;
        }
        case STATE_WAIT_BOUNCE:
        {
            if(timeouts[swIndex]>=TIMEOUT_BOUNCE)
            {
                if(ios_readSw(getPin(swIndex))==LOW)
                {
                    state[swIndex] = STATE_PRESS_CONFIRMED;             
                }
                else
                    state[swIndex] = STATE_IDLE;  
            }
            else
            {
                if(ios_readSw(getPin(swIndex))==HIGH)
                    state[swIndex] = STATE_IDLE; // bouncing   
            }
            break;
        }
        case STATE_PRESS_CONFIRMED:
        {
            // wait for short or long press
            timeouts[swIndex]=0;
            state[swIndex] = STATE_WAIT_RELEASE;
            switchesState[swIndex] = FRONT_PANEL_SW_STATE_JUST_PRESSED;
            break;
        }
        case STATE_WAIT_RELEASE:
        {
            if(ios_readSw(getPin(swIndex))==HIGH) // released
            {
                // released, check time
                if(timeouts[swIndex]<TIMEOUT_SHORT_PRESS)
                    switchesState[swIndex] = FRONT_PANEL_SW_STATE_SHORT;
                else
                    switchesState[swIndex] = FRONT_PANEL_SW_STATE_LONG;
                // wait bounce again
                timeouts[swIndex]=0;
                state[swIndex] = STATE_WAIT_BOUNCE_RELEASE;                
            }
            if(timeouts[swIndex]>TIMEOUT_LONG_PRESS)
            {
                switchesState[swIndex] = FRONT_PANEL_SW_STATE_LONG;
                state[swIndex] = STATE_WAIT_RELEASE2;
            }
            
            break;
        }
        case STATE_WAIT_RELEASE2:
        {
            if(ios_readSw(getPin(swIndex))==HIGH) // released
            {
                // wait bounce again
                timeouts[swIndex]=0;
                state[swIndex] = STATE_WAIT_BOUNCE_RELEASE;
            }          
            break; 
        }
        case STATE_WAIT_BOUNCE_RELEASE:
        {
            if(timeouts[swIndex]>=TIMEOUT_BOUNCE)
            {
              switchesState[swIndex] = FRONT_PANEL_SW_STATE_JUST_RELEASED;
              state[swIndex] = STATE_IDLE;
            }    
            break;
        }
    }
}

