//Code pertaining to driving the LEDs
#include <FastLED.h>

FASTLED_USING_NAMESPACE
CRGB leds[LED_COUNT];

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t randomHue = random(256);

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#include "GenericLEDRoutines.h"
#include "CustomLEDRoutines.h"
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { fire, pacifica, pulseStairs, rainbowWithGlitter, redCarpetLoop, pulse,  fire, pacifica ,confetti, sinelon, juggle, bpm, twinkle, runRainbow };
String routineNames[] = { "Fire", "Pacifica", "Pulse Stairs", "GLTR Rainbow",  "Red Carpet", "Pulse", "Fire", "Pacifica","Confetti", "Sinelon", "Juggle", "BPM", "Twinkle", "Step Rainbow" };
//SimplePatternList gPatterns = { pacifica, fire, pulseStairs, rainbowWithGlitter, redCarpetLoop, pulse,  confetti, sinelon, juggle, bpm, twinkle, runRainbow };
//String routineNames[] = { "Pacifica", "Fire", "Pulse Stairs", "GLTR Rainbow",  "Red Carpet", "Pulse", "Confetti", "Sinelon", "Juggle", "BPM", "Twinkle", "Step Rainbow" };
//SimplePatternList gPatterns = { pulse };
//String routineNames[] = { "pulse" };
void nextPattern()
{
  #ifdef DEBUG2
    prntln("nextPattern() called");
  #endif
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
void previousPattern()
{

  // subtract one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber - 1) < 0 ? ARRAY_SIZE( gPatterns) - 1 : gCurrentPatternNumber - 1;
}
uint8_t getCurrentPatternNumber() {
  return gCurrentPatternNumber;
}

void LEDTaskCode( void * pvParameters ){
  Serial.print("LEDTask running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    switch (currentDisplayMode) {
      case normal:
        // Call the current pattern function once, updating the 'leds' array
        gPatterns[gCurrentPatternNumber]();
      
        // send the 'leds' array out to the actual LED strip
        FastLED.show();  
        // insert a delay to keep the framerate modest
        FastLED.delay(1000/FRAMES_PER_SECOND); 
      
        // do some periodic updates
        EVERY_N_MILLISECONDS( 20 ) { 
          gHue++; // slowly cycle the "base color" through the rainbow
          nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12); //for twinkle
        } 
        EVERY_N_SECONDS( ROUTINE_CYCLE_TIME_S ) { nextPattern(); } // change patterns periodically
        EVERY_N_SECONDS( 30 ) {  // for twinkle 
          chooseNextColorPalette( gTargetPalette ); 
          randomHue = random(256);
          if (!colorManuallySelected) {
            color[0] = random(256);color[1] = random(256);color[2] = random(256);
          }
        }
      break;
      case remote:
        for(int i=0; i<LED_COUNT; i++) { 
          leds[i].setRGB(color[0],color[1],color[2]);
        }
        FastLED.show();                       
        delay(50); 
      break;
      case off:
      
        for(int i=0; i<LED_COUNT; i++) { 
          leds[i].setRGB(0,0,0);
        }
        FastLED.show();                       
        delay(50); 
      break;
      case single:
        // Call the current pattern function once, updating the 'leds' array
        gPatterns[gCurrentPatternNumber]();
      
        // send the 'leds' array out to the actual LED strip
        FastLED.show();  
        // insert a delay to keep the framerate modest
        FastLED.delay(1000/FRAMES_PER_SECOND); 
      
        // do some periodic updates
        EVERY_N_MILLISECONDS( 20 ) { 
          gHue++; // slowly cycle the "base color" through the rainbow
          nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12); //for twinkle
        } 
        EVERY_N_SECONDS( 30 ) { 
          chooseNextColorPalette( gTargetPalette ); // for twinkle 
          randomHue = random(256);
          if (!colorManuallySelected) {
            color[0] = random(256);color[1] = random(256);color[2] = random(256);
          }
        }
      break;
      case night:
      unsigned long msSinceLastMotion = millis() - motionLastDetected;
      if (msSinceLastMotion < MS_NIGHT_MODE_STAY_ON) {
        #ifdef DEBUG3
          prntln("unrolling carpet");
        #endif
        redCarpetUnroll();
      } else if (msSinceLastMotion >= MS_NIGHT_MODE_STAY_ON && msSinceLastMotion < MS_NIGHT_MODE_STAY_ON + 15000){
        #ifdef DEBUG3
          prntln("rolling up carpet");
        #endif
        redCarpetRollUp();
      }
      FastLED.show();  
      FastLED.delay(1000/FRAMES_PER_SECOND); 
      break;
    }

  } 
}

void setupLEDs() {
  delay(1000); // 1 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,LED_PIN,CLK_PIN,COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(map(brightness, 0,10,0,255));
}

void changeBrightness() {
  #ifdef DEBUG
    Serial.print("New brightness (0-255): ");
    Serial.println(map(brightness, 0,10,0,255));
  #endif
  FastLED.setBrightness(map(brightness, 0,10,0,255));
}

