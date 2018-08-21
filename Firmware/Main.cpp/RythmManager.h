#define PATTERNS_LEN      32
#define INSTRUMENTS_LEN   7


void rthm_init(void);
void rthm_tick(void);
void rthm_loop(void);

void rthm_setTempo(int tempo);
int rthm_getCurrentTempo(void);

void rthm_playCurrentPattern(void);
void rthm_playPattern(int pattern);
int rthm_getCurrentPattern(void);
void rthm_stop(void);


void rthm_incStep(void);
int rthm_getCurrentStep(void);
int rthm_isNewStepFinished(void);
void rthm_resetNewStepFinishedFlag(void);

unsigned short rthm_getPattern(unsigned char patIndex,unsigned char instrIndex);

void rthm_writeSound(unsigned char patIndex,unsigned char patternToWriteStep,unsigned char instrIndex);
void rthm_writeSilence(unsigned char patIndex,unsigned char patternToWriteStep,unsigned char instrIndex);
void rthm_cleanPattern(unsigned char patIndex,unsigned char instrIndex);


