
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

