
#define SCREEN_PLAYING  0
#define SCREEN_WRITING  1
#define SCREEN_CONFIG   2
#define SCREEN_CHAIN    3


void display_init(void);
void display_showScreen(unsigned char s);
void display_loop(void);
void display_update(void);

void display_debug(void);

