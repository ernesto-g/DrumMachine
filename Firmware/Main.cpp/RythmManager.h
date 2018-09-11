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

#define PATTERNS_LEN        32
#define INSTRUMENTS_LEN     7
#define INSTRUMENTS_LEN_WITH_ACC  (INSTRUMENTS_LEN+1)
#define PATTERNS_CHAIN_LEN  7


void rthm_init(void);
void rthm_tick(void);
void rthm_loop(void);

void rthm_setTempo(int tempo);
int rthm_getCurrentTempo(void);

void rthm_playCurrentPattern(void);
void rthm_playPattern(int pattern);
int rthm_getCurrentPattern(void);
void rthm_stop(void);


void rthm_incStep(void);
int rthm_getCurrentStep(void);
int rthm_isNewStepFinished(void);
void rthm_resetNewStepFinishedFlag(void);

unsigned short rthm_getPattern(unsigned char patIndex,unsigned char instrIndex);
void rthm_setPendingPattern(unsigned char nextPat);
signed char rthm_getPendingPattern(void);

void rthm_writeSound(unsigned char patIndex,unsigned char patternToWriteStep,unsigned char instrIndex);
void rthm_writeSilence(unsigned char patIndex,unsigned char patternToWriteStep,unsigned char instrIndex);
void rthm_cleanPattern(unsigned char patIndex);
void rthm_setEndOfPattern(unsigned char patIndex,unsigned char patIndexMax);
unsigned char rthm_getEndOfPattern(unsigned char patIndex);

void rthm_removeLastPatternInChain(void);
void rthm_addPatternToChain(unsigned char newPattern);
signed char* rthm_getPatternsChain(void);

void rthm_copyPattern(unsigned char destPattIndex,unsigned char originPattIndex);



