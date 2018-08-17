
#define INSTR_BD  0
#define INSTR_SD  1
#define INSTR_CH  2
#define INSTR_OH  3
#define INSTR_HC  4
#define INSTR_CB  5
#define INSTR_CV  6
#define INSTR_TO  7

void inst_tick(void);
void inst_init(void);
void inst_loop(void);
void inst_playInstrument(int instrumentIndex);
int inst_areAllInstrumentsIdle(void);

