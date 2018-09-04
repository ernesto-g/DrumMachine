
void mem_init(void);

void mem_savePattern(unsigned char patternIndex,unsigned char instrIndex,unsigned short pattern);
unsigned short mem_loadPattern(unsigned char patternIndex,unsigned char instrIndex);

void mem_savePatternEnd(unsigned char patternIndex,unsigned char value);
unsigned char mem_loadPatternEnd(unsigned char patternIndex);

