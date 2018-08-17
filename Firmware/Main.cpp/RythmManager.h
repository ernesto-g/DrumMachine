#define PATTERNS_LEN      32
#define INSTRUMENTS_LEN   8


void rthm_init(void);
void rthm_tick(void);
void rthm_loop(void);

void rthm_setTempo(int tempo);
int rthm_getCurrentTempo(void);

void rthm_playCurrentPattern(void);
void rthm_playPattern(int pattern);

void rthm_incStep(void);
int rthm_getCurrentStep(void);
int rthm_isNewStepFinished(void);
void rthm_resetNewStepFinishedFlag(void);
