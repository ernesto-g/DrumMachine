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
#include "Icons.h"

const unsigned char PROGMEM logo16_play_bmp[] =
{ B10000000, B00000000,
B11100000, B00000000,
B11111000, B00000000,
B11111100, B00000000,
B11111111, B00000000,
B11111111, B11000000,
B11111111, B11110000,
B11111111, B11111100,
B11111111, B11111100,
B11111111, B11110000,
B11111111, B11000000,
B11111111, B00000000,
B11111100, B00000000,
B11111000, B00000000,
B11100000, B00000000,
B10000000, B00000000 };

const unsigned char PROGMEM logo16_writing_bmp[] =
{ B00000000, B00000000,
B01111111, B11111110,
B01000000, B00100010,
B01000000, B00100010,
B01000000, B00111110,
B01000001, B10000010,
B01000100, B01000010,
B01001001, B00100010,
B01000100, B01000010,
B01000001, B10000010,
B01000000, B00000010,
B01000000, B00000010,
B01000000, B00000010,
B01000000, B00000010,
B01111111, B11111110,
B00000000, B00000000 };



