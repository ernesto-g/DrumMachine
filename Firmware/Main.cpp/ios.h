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

#define PIN_TRIGGER_ACC    6

#define PIN_SW_ENTER      2
#define PIN_SW_ESC        3
#define PIN_SW_SHIFT      5
#define PIN_SW_PLAY_WRITE 4



void ios_setLo(int triggerNumber);
void ios_setHi(int triggerNumber);
int ios_readSw(int swPin);

void ios_init(void);


