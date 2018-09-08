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

#define MIDI_CMD_NOTE_ON  144
#define MIDI_CMD_NOTE_OFF  128
#define MIDI_CMD_CONTROL_CHANGE 0xB0

#define MIDI_STATE_IDLE       0
#define MIDI_STATE_RVC_DATA1  1
#define MIDI_STATE_RVC_DATA2  2

#define MIDI_BYTE_TIMEOUT   200 // 200ms

typedef struct S_MidiInfo {
  unsigned char channel;
  unsigned char cmd;
  unsigned char note;
  unsigned char vel;
}MidiInfo;


void midi_stateMachine(unsigned char midiByte);
void midi_loop(void);
void midi_init(void);
void midi_tickMs(void);
void midi_setMidiChn(unsigned char val);
unsigned char midi_getMidiChn(void);








