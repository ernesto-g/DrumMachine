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

static const char INSTRUMENTS_NAMES[7][3]={"BD","SD","CH","OH","HC","CV","TO"};
static unsigned char flagRedrawScreen;
static unsigned char currentScreen;

static void showPlayingScreen(void);
static void showWritingScreen(void);
static void showConfigScreen(void);

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
        }
          
        display.display(); // draw screen
    }
}


void display_update(void)
{
    flagRedrawScreen=1;
}


static void showPlayingScreen(void)
{
    int currentStep = rthm_getCurrentStep();
    int currentTempo = rthm_getCurrentTempo();

    display.clearDisplay();  

    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.drawBitmap(0, 16,  logo16_play_bmp, 16, 16, 1);

    display.setCursor(40,0);
    display.print("STEP:");
    display.print(currentStep);
    
    display.setCursor(40,16);
    display.print(currentTempo);
    
    display.setTextSize(1);
    display.setCursor(80,16);
    display.print("bpm");
    
    display.setCursor(17,24);
    display.print("sh");
}

static void showWritingScreen(void)
{
    int currentPattern = rthm_getCurrentPattern();
    
    display.clearDisplay();  
    
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(INSTRUMENTS_NAMES[logic_getSelectedInstrument()]);
    
    display.drawBitmap(0, 16,  logo16_writing_bmp, 16, 16, 1);
    // shift sw agregar if
    display.setCursor(17,24);
    display.print("sh");  
    //_________
    
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40,0);
    display.print("PATT:");
    display.print(currentPattern);
}

static void showConfigScreen(void)
{
    display.clearDisplay();  
  
}


