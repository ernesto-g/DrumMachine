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

#define IOS_TRIGGER_BD    0
#define IOS_TRIGGER_SD    1
#define IOS_TRIGGER_CH    2 
#define IOS_TRIGGER_OH    3
#define IOS_TRIGGER_HC_1  4
#define IOS_TRIGGER_HC_2  5
#define IOS_TRIGGER_CB_CV 6 
#define IOS_TRIGGER_T0    7
#define IOS_TRIGGER_ACC    8


#define PIN_TRIGGER_BD    13
#define PIN_TRIGGER_SD    12
#define PIN_TRIGGER_CH    11 
#define PIN_TRIGGER_OH    10
#define PIN_TRIGGER_HC_1  9
#define PIN_TRIGGER_HC_2  8
#define PIN_TRIGGER_CB_CV 7 
#define PIN_TRIGGER_T0    6

#define PIN_TRIGGER_ACC    14

#define PIN_SW_ENTER      2
#define PIN_SW_ESC        3
#define PIN_SW_SHIFT      5
#define PIN_SW_PLAY_WRITE 4



void ios_setLo(int triggerNumber);
void ios_setHi(int triggerNumber);
int ios_readSw(int swPin);

void ios_init(void);


