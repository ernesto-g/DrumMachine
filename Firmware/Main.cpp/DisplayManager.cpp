#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include "RythmManager.h"
#include "DisplayManager.h"
#include "Icons.h"
#include "Logic.h"

// icons
extern const unsigned char PROGMEM logo16_play_bmp[];
extern const unsigned char PROGMEM logo16_writing_bmp[];
//______

#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 32)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

static const char INSTRUMENTS_NAMES[INSTRUMENTS_LEN+1][3]={"BD","SD","CH","OH","CP","CL","TO","AC"};
static unsigned char flagRedrawScreen;
static unsigned char currentScreen;

static void showPlayingScreen(void);
static void showWritingScreen(void);
static void showConfigScreen(void);
static void showChainScreen(void);


void display_init(void)
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  
  // Clear the buffer.
  display.clearDisplay();  
  flagRedrawScreen=1;
}

void display_showScreen(unsigned char s)
{
    currentScreen = s;
}

void display_loop(void)
{
    if(flagRedrawScreen==1)
    {
        flagRedrawScreen=0;
        // it takes 35ms
        switch(currentScreen)
        {
            case SCREEN_PLAYING:
            {
                showPlayingScreen();
                break;
            }
            case SCREEN_WRITING:
            {
                showWritingScreen();
                break;
            }
            case SCREEN_CONFIG:
            {
                showConfigScreen();
                break;
            }
            case SCREEN_CHAIN:
            {
                showChainScreen();
            }
        }
          
        display.display(); // draw screen
    }
}


void display_update(void)
{
    flagRedrawScreen=1;
}

static void showChainScreen(void)
{    
    unsigned char i;
    signed char* chain = rthm_getPatternsChain();

    display.clearDisplay();  

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("NEXT:");
    display.print(logic_getPatternForChain());
    
    display.setTextSize(1);
    display.setCursor(0,16);
    display.print("PATTERNS CHAIN:");
    display.setCursor(0,24);

    for(i=0; i<PATTERNS_CHAIN_LEN; i++)
    {
        if(chain[i]!=-1)
        {
          display.print(chain[i]);
          display.print(" ");  
        }
        else
          break;
    }
    
    
    // shift sw
    display.setTextSize(1);
    display.setCursor(110,0);
    if(logic_getSwShiftState())
      display.print("sh");
    //_________
}

static void showPlayingScreen(void)
{
    int currentStep = rthm_getCurrentStep();
    int currentTempo = rthm_getCurrentTempo();
    int currentPattern = rthm_getCurrentPattern();
    
    display.clearDisplay();  

    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.setCursor(0,0);
    if(currentPattern<10)
      display.print("0");
    display.print(currentPattern);
    
    display.drawBitmap(0, 16,  logo16_play_bmp, 16, 16, 1);

    display.setCursor(40,0);
    display.print("STEP:");
    if(currentStep<10)
      display.print("0");
    display.print(currentStep);
    
    display.setCursor(40,16);
    if(currentTempo<100)
      display.print("0");
    display.print(currentTempo);
    
    display.setTextSize(1);
    display.setCursor(80,16);
    display.print("bpm");
    
    // shift sw
    display.setTextSize(1);
    display.setCursor(17,16);
    if(logic_getSwShiftState())
      display.print("sh");
    //_________
}

static void showWritingScreen(void)
{
    int wPattern = logic_getWritingPattern();
    int wPatternStep = logic_getWritingPatternStep();
    
    display.clearDisplay();  
    
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(INSTRUMENTS_NAMES[logic_getSelectedInstrument()]);
    
    //display.drawBitmap(0, 16,  logo16_writing_bmp, 16, 16, 1);
    
    display.setTextSize(2);
    display.setTextColor(WHITE);

    // step to write
    display.setCursor(32,0);
    display.setTextSize(1);
    display.print("ST:");
    display.setTextSize(2);
    if(wPatternStep<10)
      display.print("0");
    display.print(wPatternStep);

    // pattern to write
    display.setCursor(80,0);
    display.setTextSize(1);
    display.print("PT:");
    display.setTextSize(2);
    if(wPattern<10)
      display.print("0");
    display.print(wPattern);


    // shift sw
    display.setTextSize(1);
    display.setCursor(26,11);
    if(logic_getSwShiftState())
      display.print("sh");
    //_________

    
    // draw pattern to write
    display.drawLine(0, 30, 127, 30, WHITE);
    unsigned short patt = rthm_getPattern(wPattern,logic_getSelectedInstrument());
    unsigned char i;
    for(i=0; i<16; i++)
    {
        if( (patt&0x0001)==0x0001)
          display.fillRect( (i*8) + 2, 23, 4, 6, WHITE);
        else
          display.drawRect( (i*8) + 2, 23, 4, 6, WHITE);

       patt=patt>>1;
    }
    //_____________________    

}

static void showConfigScreen(void)
{
    display.clearDisplay();  
  
}


