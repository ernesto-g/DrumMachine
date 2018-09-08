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

#define INSTR_BD  0
#define INSTR_SD  1
#define INSTR_CH  2
#define INSTR_OH  3
#define INSTR_CP  4
#define INSTR_CL  5
#define INSTR_TO  6
#define INSTR_ACC  7

void inst_tick(void);
void inst_init(void);
void inst_loop(void);
void inst_playInstrument(int instrumentIndex);
int inst_areAllInstrumentsIdle(void);

void inst_accOn(void);
void inst_accOff(void);

