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


#define FRONT_PANEL_SW_STATE_IDLE           0
#define FRONT_PANEL_SW_STATE_JUST_PRESSED   1
#define FRONT_PANEL_SW_STATE_SHORT          2
#define FRONT_PANEL_SW_STATE_LONG           3
#define FRONT_PANEL_SW_STATE_JUST_RELEASED  4


#define SW_ENTER        0
#define SW_ESC          1
#define SW_SHIFT        2
#define SW_PLAY_WRITE   3

void frontp_init(void);
void frontp_loop(void);
void frontp_tick1Ms(void);

int frontp_getSwState(int swIndex);
void frontp_resetSwState(int swIndex);

int frontp_getEncoderPosition(void);
void frontp_setEncoderPosition(int pos);

