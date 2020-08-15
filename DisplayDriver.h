//code pertaining to driving the LCD display
#include "SPI.h"
#include "TFT_eSPI.h"
// The custom font file attached to this sketch must be included
#include "MyFont.h"
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include "NotoSans24.h"
// Stock font and GFXFF reference handle
#define GFXFF 1
// Easily remembered name for the font
#define MYFONT32 &myFont32pt8b
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_MED NotoSans24
#define AA_FONT_LARGE NotoSansBold36
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();
void updateDisplay();

void setupDisplay() {
  tft.begin();
  tft.setRotation(0);
}

word convertRGB( byte R, byte G, byte B)
{
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

//these variables are to keep track of the last displayed values. If theres no change, dont update display
displayMode currentDisplayModeLastDisplayed;
byte brightnessLastDisplayed;
bool motionDetectedLastDisplayed;
int colorLastDisplayed[3];
uint8_t routineLastDisplayed;

bool doesDisplayNeedUpdating() {
  bool needsUpdating = false;
  if (currentDisplayMode != currentDisplayModeLastDisplayed || 
    brightness != brightnessLastDisplayed ||
    motionDetected != motionDetectedLastDisplayed) {
      needsUpdating = true;
      currentDisplayModeLastDisplayed = currentDisplayMode;
      brightnessLastDisplayed = brightness;
      motionDetectedLastDisplayed = motionDetected;
  }
  if (getCurrentPatternNumber() != routineLastDisplayed) {
    routineLastDisplayed = getCurrentPatternNumber();
    needsUpdating = true;
  }
  for (int i = 0; i < 3; i++) {
    if (color[i] != colorLastDisplayed[i]) {
      needsUpdating = true;
      colorLastDisplayed[i] = color[i];
    }
  }
  return needsUpdating;
}
void updateDisplay() {
  //only update screen if there is new info to show
  if (!doesDisplayNeedUpdating())
    return;
  tft.loadFont(AA_FONT_SMALL);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.println("LED Kommander"); // println moves cursor down for a new line
  tft.println(); // New line
  tft.println("Mode: ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.loadFont(AA_FONT_MED);
  switch (currentDisplayMode) {
    case normal:
      tft.println("NORMAL");
      tft.loadFont(AA_FONT_SMALL);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.println("Displaying:");
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.print("#");
      tft.print(getCurrentPatternNumber());
      tft.print(": ");
      tft.println(routineNames[getCurrentPatternNumber()]);
    break;
    case night:
      tft.println("NIGHT");
    break;
    case off:
       tft.loadFont(AA_FONT_LARGE);
       tft.println("OFF");
       return;
    break;
    case wifi:
       tft.println("WIFI");
    break;
    case remote:
       tft.println("COLOR");
       tft.loadFont(AA_FONT_SMALL);
       tft.setTextColor(convertRGB(color[0],color[1],color[2]), TFT_BLACK);
       tft.println("Displaying Color:");
       tft.print(color[0]);
       tft.print(",");
       tft.print(color[1]);
       tft.print(",");
       tft.println(color[2]);
    break;
    case single:
      tft.println("CONSTANT");
      tft.loadFont(AA_FONT_SMALL);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.println("Displaying:");
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.print("#");
      tft.print(getCurrentPatternNumber());
      tft.print(": ");
      tft.println(routineNames[getCurrentPatternNumber()]);
    break;
    default:
       tft.println("UNKNOWN");
    break;
  }
  tft.loadFont(AA_FONT_SMALL);
  tft.println();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Brightness: ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(brightness);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  if (motionDetected) {
    tft.println("Motion Detected");   
  } else {
    tft.println("No Motion");
    #ifdef DEBUG3
      tft.print(((millis() - motionLastDetected)/3600000)/1000);
      tft.print("h ");
      tft.print((((millis() - motionLastDetected)%3600000)/60)/1000);
      tft.print("m "); 
      tft.print(((millis() - motionLastDetected)%60000)/1000);
      tft.println("s "); 
    #endif
  }
  

}
