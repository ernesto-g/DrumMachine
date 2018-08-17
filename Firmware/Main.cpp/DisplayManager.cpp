#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include "RythmManager.h"
#include "DisplayManager.h"

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
static const unsigned char PROGMEM logo16_play_bmp[] =
{ B10000000, B00000000,
B11100000, B00000000,
B11111000, B00000000,
B11111100, B00000000,
B11111111, B00000000,
B11111111, B11000000,
B11111111, B11110000,
B11111111, B11111100,
B11111111, B11111100,
B11111111, B11110000,
B11111111, B11000000,
B11111111, B00000000,
B11111100, B00000000,
B11111000, B00000000,
B11100000, B00000000,
B10000000, B00000000 };

static const unsigned char PROGMEM logo16_writing_bmp[] =
{ B00000000, B00000000,
B01111111, B11111110,
B01000000, B00100010,
B01000000, B00100010,
B01000000, B00111110,
B01000001, B10000010,
B01000100, B01000010,
B01001001, B00100010,
B01000100, B01000010,
B01000001, B10000010,
B01000000, B00000010,
B01000000, B00000010,
B01000000, B00000010,
B01000000, B00000010,
B01111111, B11111110,
B00000000, B00000000 };


static const char INSTRUMENTS_NAMES[7][3]={"BD","SD","CH","OH","HC","CV","TO"};
static int flagRedrawScreen;

void display_init(void)
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  
  // Clear the buffer.
  display.clearDisplay();  
  flagRedrawScreen=1;
}

void display_loop(void)
{
    if(flagRedrawScreen==1)
    {
        flagRedrawScreen=0;
        // Poner switch con diferentes pantallas
        // tarda 35ms
        display_showMainScreen();
        display.display(); // draw screen
    }
}


void display_update(void)
{
    flagRedrawScreen=1;
}
void display_showMainScreen(void)
{
    int currentStep = rthm_getCurrentStep();
    int currentTempo = rthm_getCurrentTempo();

    display.clearDisplay();  

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(INSTRUMENTS_NAMES[0]);

    display.drawBitmap(0, 16,  logo16_play_bmp, 16, 16, 1);
    //display.drawBitmap(0, 16,  logo16_writing_bmp, 16, 16, 1);

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

