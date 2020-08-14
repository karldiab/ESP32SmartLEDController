//Code pertaining to driving the LEDs
#include <FastLED.h>

FASTLED_USING_NAMESPACE
CRGB leds[LED_COUNT];

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#include "LEDRoutines.h"
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void LEDTaskCode( void * pvParameters ){
  Serial.print("LEDTask running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    if (currentDisplayMode == normal) {
      // Call the current pattern function once, updating the 'leds' array
      gPatterns[gCurrentPatternNumber]();
    
      // send the 'leds' array out to the actual LED strip
      FastLED.show();  
      // insert a delay to keep the framerate modest
      FastLED.delay(1000/FRAMES_PER_SECOND); 
    
      // do some periodic updates
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
      EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
//      #ifdef DEBUG
//        Serial.println("Running juggle routine");
//      #endif
//      juggle();
//      FastLED.show();  
//      // insert a delay to keep the framerate modest
//      FastLED.delay(1000/FRAMES_PER_SECOND); 
//      #ifdef DEBUG
//        Serial.println("Running bpm routine");
//      #endif
//      bpm();
//      FastLED.show();  
//      // insert a delay to keep the framerate modest
//      FastLED.delay(1000/FRAMES_PER_SECOND); 
//      #ifdef DEBUG
//        Serial.println("Running sinelon routine");
//      #endif
//      sinelon();
//      FastLED.show();  
//      // insert a delay to keep the framerate modest
//      FastLED.delay(1000/FRAMES_PER_SECOND); 
//      #ifdef DEBUG
//        Serial.println("Running confetti routine");
//      #endif
//      confetti();
//      FastLED.show();  
//      // insert a delay to keep the framerate modest
//      FastLED.delay(1000/FRAMES_PER_SECOND); 
//      #ifdef DEBUG
//        Serial.println("Running rainbowWithGlitter routine");
//      #endif
//      rainbowWithGlitter();
//      FastLED.show();  
//      // insert a delay to keep the framerate modest
//      FastLED.delay(1000/FRAMES_PER_SECOND); 
//      #ifdef DEBUG
//        Serial.println("Running rainbow routine");
//      #endif
//      rainbow();
//      FastLED.show();  
//      // insert a delay to keep the framerate modest
//      FastLED.delay(1000/FRAMES_PER_SECOND); 
      
    } else if (currentDisplayMode == remote) {
      #ifdef DEBUG
        Serial.println("Tryin to do remote shit");  
        Serial.print("RGB: ");
        Serial.print(color[0]);
        Serial.print(" ");
        Serial.print(color[1]);
        Serial.print(" ");
        Serial.println(color[2]);
      #endif
    for(int i=0; i<LED_COUNT; i++) { 
        //strip.setPixelColor(i, strip.Color(color[0], color[1],color[2]));       
      }
      //strip.show();                          
      //strip.fill(strip.Color(color[0], color[1],color[2]), 0, LED_COUNT);
      remoteCommandInQueue = false;
      delay(100);               
    }
  } 
}

void setupLEDs() {
  delay(1000); // 1 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,LED_PIN,CLK_PIN,COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

