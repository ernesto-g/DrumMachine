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


#define SCREEN_PLAYING      0
#define SCREEN_WRITING      1
#define SCREEN_CONFIG       2
#define SCREEN_CHAIN        3
#define SCREEN_COPY_PATTERN 4

void display_init(void);
void display_showScreen(unsigned char s);
void display_loop(void);
void display_update(void);

void display_debug(void);

