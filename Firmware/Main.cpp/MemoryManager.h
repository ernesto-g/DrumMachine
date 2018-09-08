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

void mem_init(void);

void mem_savePattern(unsigned char patternIndex,unsigned char instrIndex,unsigned short pattern);
unsigned short mem_loadPattern(unsigned char patternIndex,unsigned char instrIndex);

void mem_savePatternEnd(unsigned char patternIndex,unsigned char value);
unsigned char mem_loadPatternEnd(unsigned char patternIndex);

unsigned char mem_loadMidiChn(void);
void mem_saveMidiChn(unsigned char val);


