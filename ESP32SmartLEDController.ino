
//display stuff
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


//LED STRIP SHIT
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    12
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 100
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//global variables to keep track of settings
enum displayMode {
  normal,
  night,
  off,
  wifi,
  remote
};
displayMode currentDisplayMode = normal;
//volitile because it is accessed in interupt function
volatile bool motionDetected = false;
volatile unsigned long motionLastDetected = 0;
volatile unsigned long screenLastUpdated = 0;

//Motion detector setup
#define MOTION_PIN 27
hw_timer_t * myTimer = NULL; 
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//IR Receiver setup

#include <IRremote.h>
#define IR_RECEIVE_PIN 13
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;        // Somewhere to store the results
void decodeIRCommand(unsigned int value);
#define MAX_BRIGHTNESS_VALUE 16
int brightness = 8; //value that ranges from 0 to 15
int color[3] = {255,255,255}; //RGB values from 
bool systemOn = true;
bool remoteCommandInQueue = false;


#define button_brightnessUp 16187647
#define button_brightnessDown 16220287
#define button_off 16203967
#define button_on 16236607
#define button_red 16195807
#define button_green 16228447
#define button_blue 16212127
#define button_white 16244767
#define button_orange 16191727
#define button_aquamarine 16224367
#define button_navy 16208047
#define button_flash 16240687
#define button_darkOrange 16199887
#define button_turquoise 16232527
#define button_darkPurple 16216207
#define button_strobe 16248847
#define button_brightOrange 16189687
#define button_mediumTurquoise 16222327
#define button_purple 16206007
#define button_fade 16238647
#define button_yellow 16197847
#define button_darkTurquoise 16230487
#define button_pink 16214167
#define button_smooth 16246807

//timer interupt function to check status of motion detector
void IRAM_ATTR ISR2() {
  //check if got new remote command
  if (irrecv.decode(&results) && !remoteCommandInQueue) {  // Grab an IR code
    //Serial.println(results.value, DEC);
    irrecv.resume(); // Receive the next value
    decodeIRCommand(results.value);
  }
  //check motion detector
  if(digitalRead(MOTION_PIN)==HIGH) {
    portENTER_CRITICAL_ISR(&timerMux);
    motionDetected = true;
    motionLastDetected = millis();
    portEXIT_CRITICAL_ISR(&timerMux);
  } else {
    portENTER_CRITICAL_ISR(&timerMux);
    motionDetected = false;
    portEXIT_CRITICAL_ISR(&timerMux);
  }
  //update display if its been long enough
  if ((millis() - screenLastUpdated) > 2500) {
    screenLastUpdated = millis();
    updateDisplay();
  }
}



void setup(void) {
  Serial.begin(115200);
  //for motion detector
  pinMode(MOTION_PIN, INPUT);
//setup timer interupt firing every 100 ms to check status of motion detector
  myTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(myTimer, &ISR2, true);
  timerAlarmWrite(myTimer, 100000, true);
  timerAlarmEnable(myTimer);
  
  //for lcd
  tft.begin();
  tft.setRotation(0);
  tft.loadFont(AA_FONT_SMALL);

  //for LED controls
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(200); // Set BRIGHTNESS to about 1/5 (max = 255)

  //ir receiver
  irrecv.enableIRIn();  // Start the receiver
}
void pulseColor(int R, int G, int B, int wait);
void rainbow(int wait);
void loop() {
  //updateDisplay();
  //LEDS
  if (currentDisplayMode == normal) {
    
    pulseColor(255,255,255,20);
    rainbow(10);   
  } else if (currentDisplayMode == remote) {
    Serial.println("Tryin to do remote shit");
//    Serial.print("RGB: ");
//    Serial.print(color[0]);
//    Serial.print(" ");
//    Serial.print(color[1]);
//    Serial.print(" ");
//    Serial.println(color[2]);
  for(int i=0; i<strip.numPixels(); i++) { 
      strip.setPixelColor(i, strip.Color(color[0], color[1],color[2]));       
    }
    strip.show();                          
    strip.fill(strip.Color(color[0], color[1],color[2]), 0, LED_COUNT);
    remoteCommandInQueue = false;
    delay(100);               
  }

    
//  // Fill along the length of the strip in various colors...
//  colorWipe(strip.Color(255,   0,   0), 50); // Red
//  colorWipe(strip.Color(  0, 255,   0), 50); // Green
//  colorWipe(strip.Color(  0,   0, 255), 50); // Blue
//
//  // Do a theater marquee effect in various colors...
//  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
//  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness
//
//  rainbow(10);             // Flowing rainbow cycle along the whole strip
//  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant

//    Serial.print("RGB values displaying: ");
//    Serial.print((color[0]*brightness)/MAX_BRIGHTNESS_VALUE);
//    Serial.print((color[1]*brightness)/MAX_BRIGHTNESS_VALUE);
//    Serial.println((color[2]*brightness)/MAX_BRIGHTNESS_VALUE);
//  if (receiver.decode(&output)) { 
//    unsigned int value = output.value;
//    Serial.println(value);
//    decodeIRCommand(value);
//    receiver.resume();
//  }
}


// Some functions of our own for creating animated effects -----------------

void pulseColor(int R, int G, int B, int wait) {
  uint32_t colorDimmest = strip.Color(R/32,G/32,B/32);
  uint32_t colorDimmeerer = strip.Color(R/16,G/16,B/16);
  uint32_t colorDimmer = strip.Color(R/8,G/8,B/8);
  uint32_t colorDim = strip.Color(R/4,G/4,B/4);
  uint32_t color = strip.Color(R,G,B);
  uint32_t black = strip.Color(0,0,0);
  for(int i=0; i<strip.numPixels()+6; i++) { 
    if (currentDisplayMode != normal) {
      return;
    }
    for (int j=0; j<5;j++) {
      strip.setPixelColor(i-6, black);
      strip.setPixelColor(i-5, colorDimmest);
      strip.setPixelColor(i+5, colorDimmest);  
      strip.setPixelColor(i-4, colorDimmeerer);
      strip.setPixelColor(i+4, colorDimmeerer);   
      strip.setPixelColor(i-3, colorDimmer);
      strip.setPixelColor(i+3, colorDimmer);  
      strip.setPixelColor(i-1, colorDim);
      strip.setPixelColor(i+1, colorDim);
      strip.setPixelColor(i, color);       
      strip.show();                          
      delay(wait);  
    }
                         
  }
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


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
     if (currentDisplayMode != normal) {
      return;
      }
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

//ir receiver decode functions
void decodeIRCommand(unsigned int value) {
  switch (value) {
    case button_off:
      turnOff();
      systemOn = false;
      color[0] = 0;
      color[1] = 0;
      color[2] = 0;
    break;
    case button_on:
      systemOn = true;
    break;
    case button_red:
      systemOn = true;
      color[0] = 255;
      color[1] = 0;
      color[2] = 0;
    break;
    case button_green:
      systemOn = true;
      color[0] = 0;
      color[1] = 255;
      color[2] = 0;
    break;
    case button_blue:
      systemOn = true;
      color[0] = 0;
      color[1] = 0;
      color[2] = 255;
    break;
    case button_white:
      systemOn = true;
      color[0] = 255;
      color[1] = 255;
      color[2] = 255;
    break;
    case button_brightnessUp:
      systemOn = true;
      if (brightness < MAX_BRIGHTNESS_VALUE)
        brightness++;
    break;
    case button_brightnessDown:
      systemOn = true;
      if (brightness > 0)
        brightness--;
    break;
    case button_orange:
      systemOn = true;
      color[0] = 255;
      color[1] = 60;
      color[2] = 0;
    break;
    case button_aquamarine:
      systemOn = true;
      color[0] = 32;
      color[1] = 210;
      color[2] = 137;
    break;
    case button_navy:
      systemOn = true;
      color[0] = 0;
      color[1] = 0;
      color[2] = 128;
    break;
    case button_darkOrange:
      systemOn = true;
      color[0] = 255;
      color[1] = 45;
      color[2] = 0;
    break;
    case button_turquoise:
      systemOn = true;
      color[0] = 64;
      color[1] = 224;
      color[2] = 208;
    break;
    case button_darkPurple:
      systemOn = true;
      color[0] = 128;
      color[1] = 0;
      color[2] = 128;
    break;
    case button_brightOrange:
      systemOn = true;
      color[0] = 255;
      color[1] = 100;
      color[2] = 0;
    break;
    case button_mediumTurquoise:
      systemOn = true;
      color[0] = 72;
      color[1] = 209;
      color[2] = 204;
    break;
    case button_purple:
      systemOn = true;
      color[0] = 128;
      color[1] = 0;
      color[2] = 156;
    break;
    case button_yellow:
      systemOn = true;
      color[0] = 255;
      color[1] = 165;
      color[2] = 0;
    break;
    case button_darkTurquoise:
      systemOn = true;
      color[0] = 0;
      color[1] = 206;
      color[2] = 209;
    break;
    case button_pink:
      systemOn = true;
      color[0] = 255;
      color[1] = 20;
      color[2] = 147;
    break;
    default:
      Serial.println("I dunno wut dat is");
      return;
    break;
  }
  Serial.println("Yayyy I understood that");
  remoteCommandInQueue = true;
  currentDisplayMode = remote;
}

void turnOff() {
      systemOn = false;
      color[0] = 0;
      color[1] = 0;
      color[2] = 0;
}
