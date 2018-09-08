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








