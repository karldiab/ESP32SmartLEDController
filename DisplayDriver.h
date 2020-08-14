//code pertaining to driving the LCD display
#include "SPI.h"
#include "TFT_eSPI.h"
// The custom font file attached to this sketch must be included
#include "MyFont.h"
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
// Stock font and GFXFF reference handle
#define GFXFF 1
// Easily remembered name for the font
#define MYFONT32 &myFont32pt8b
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();
void updateDisplay();

void setupDisplay() {
  tft.begin();
  tft.setRotation(0);
  tft.loadFont(AA_FONT_SMALL);
}
void updateDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
  tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.println("LED Kommander"); // println moves cursor down for a new line
  tft.println(); // New line
  tft.print("Mode: "); // print leaves cursor at end of line
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(currentDisplayMode); // Added to line after ABC
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println();
  if (motionDetected) {
    tft.println("Motion detected");   
  } else {
    tft.println("No motion for:");
    tft.print(((millis() - motionLastDetected)/3600000)/1000);
    tft.print("h ");
    tft.print((((millis() - motionLastDetected)%3600000)/60)/1000);
    tft.print("m "); 
    tft.print(((millis() - motionLastDetected)%60000)/1000);
    tft.println("s "); 
  }
//
//
//  tft.println(); // New line
//  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
//  float fvalue = 1.23456;
//  tft.println(fvalue, 0);  // no decimal places
//  tft.println(fvalue, 1);  // 1 decimal place
//  tft.println(fvalue, 2);  // 2 decimal places
//  tft.println(fvalue, 5);  // 5 decimal places
}
