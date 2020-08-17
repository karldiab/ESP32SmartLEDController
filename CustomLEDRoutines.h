/*These are LED routines tailored to my custom LED layout, ie deployed on my spiral staircase and around my glass fixtures
 * The LED numbers and organization are hardcoded according to my system. These routines may work for you, but probably
 * won't look good because you'll have a different layout
 */

 // progmem variables to organize my own LED layout
struct staircaseStep {
  int firstIndex; //first index, inclusive
  int lastIndex; //last index, inclusive
  bool orderReversed; //do the leds run backwards?
};
#define NUMBER_OF_STAIRS 9
const staircaseStep  stairs[NUMBER_OF_STAIRS] PROGMEM = {
  {0,10,false},
  {11,22,true},
  {23,34,false},
  {35,46,true},
  {47,58,false},
  {59,70,true},
  {71,82,false},
  {83,94,true},
  {95,106,false}
};


#define RUN_RAINBOW_TOTAL_FRAMES 1200
const byte rainbowColors[7][3] PROGMEM = {
  {255,0,0},{255,127,0},{255,255,0},{0,255,0},{0,0,255},{75,0,130},{148,0,211}
};
const byte rainbowColorsReverse[7][3] PROGMEM = {
  {148,0,211},{75,0,130},{0,0,255},{0,255,0},{255,255,0},{255,127,0},{255,0,0}
};
int runRainbowFrameNumber = 0;
void runRainbow() {
  if (runRainbowFrameNumber > RUN_RAINBOW_TOTAL_FRAMES) {
    runRainbowFrameNumber = 0;
  }
  int stepNo = runRainbowFrameNumber/(RUN_RAINBOW_TOTAL_FRAMES/NUMBER_OF_STAIRS);
  for (int stair = 0; stair < NUMBER_OF_STAIRS; stair++) { 
    int rainbowIndex = (stair+stepNo)%7;
    #ifdef DEBUG5
      Serial.print("Stairno: ");
      Serial.print(stair);
      Serial.print(" rainbowindex: ");
      Serial.println(rainbowIndex);
    #endif
    for (int led = stairs[stair].firstIndex; led <= stairs[stair].lastIndex; led++) {
      leds[led].setRGB(rainbowColors[(stair+stepNo)%7][0],rainbowColors[(stair+stepNo)%7][1],rainbowColors[(stair+stepNo)%7][2]);
    }
  }
  #ifdef DEBUG5
    Serial.println();
  #endif
  runRainbowFrameNumber++;
}

//pulse stairs
//int pulseStairsFrameNumber = 0;
//#define PULSE_STAIRS_TOTAL_FRAMES 255
//#define PULSE_STAIRS_WAVE_WIDTH 10
//int calculateWaveformBrightness(int input, int ledNo, int wavePosition, int waveWidth, int stair);
////lights up all the stairs at the same time by pulsing out light from the center
//double mapf(double x, double in_min, double in_max, double out_min, double out_max);
//void pulseStairs() {
//  int wavePosition;
//  if (pulseStairsFrameNumber > PULSE_STAIRS_TOTAL_FRAMES) {
//    pulseStairsFrameNumber = 0;
//  }
//  #ifdef DEBUG5
//    Serial.print("Frame number: ");
//    Serial.println(pulseStairsFrameNumber);
//  #endif
//  for (int stair = 0; stair < NUMBER_OF_STAIRS; stair++) {
//    if (!stairs[stair].orderReversed) {
//      //calculate where the start of the wave should
//      wavePosition = map(pulseStairsFrameNumber, 0, PULSE_STAIRS_TOTAL_FRAMES, stairs[stair].firstIndex-PULSE_STAIRS_WAVE_WIDTH,stairs[stair].lastIndex);
//      double waveProgress = (double)pulseStairsFrameNumber/PULSE_STAIRS_TOTAL_FRAMES;
//      //wavePosition = getWavePosition();
//      #ifdef DEBUG5
//        if (stair == 0) {
//          Serial.print("wavePosition: ");
//          Serial.println(wavePosition);
//        }
//
//      #endif
//      for (int led = stairs[stair].firstIndex; led <= stairs[stair].lastIndex; led++) {
//        
//        //check if this LED is one of the ones that should be illuminated
//        if (led >= wavePosition && led < wavePosition + PULSE_STAIRS_WAVE_WIDTH) {
//
//          int ledBrightness = 
//          double brightnessFactor = mapf(color
//          int colorZeroBrightness = calculateWaveformBrightness(color[0],led,wavePosition,PULSE_STAIRS_WAVE_WIDTH, stair);
//          leds[led].setRGB(colorZeroBrightness,
//            calculateWaveformBrightness(color[1],led,wavePosition,PULSE_STAIRS_WAVE_WIDTH, stair),
//            calculateWaveformBrightness(color[2],led,wavePosition,PULSE_STAIRS_WAVE_WIDTH, stair));
//        #ifdef DEBUG5
//          if (stair == 0) {
//            Serial.print("led: ");
//            Serial.print(led);
//            Serial.print(" wavePosition: ");
//            Serial.print(wavePosition);
//            Serial.print(" brightness: ");
//            Serial.println(colorZeroBrightness);
//          }
//        #endif
//        } else {
//          //if its not, set to black;
//          leds[led].setRGB(0,0,0);
//        }
//      }
//    }
//  }
//  
//  #ifdef DEBUG5
//  
//    Serial.println();
//  #endif
//  pulseStairsFrameNumber++;
//}
//
////int getWavePosition(int pulseStairsFrameNumber, ) {
////  wavePosition = map(pulseStairsFrameNumber, 0, PULSE_STAIRS_TOTAL_FRAMES, stairs[stair].firstIndex-PULSE_STAIRS_WAVE_WIDTH,stairs[stair].lastIndex);
////}
////calculates waveform brightness by multiplying input (0-255) with the cos value (half wave mapped to -1/2pi to 1/2pi) of the specific leds position in the wave using the wave description input params
//int calculateWaveformBrightness(int input, int ledNo, int wavePosition, int waveWidth, int stair) {
//  double waveProgress = (double)pulseStairsFrameNumber/PULSE_STAIRS_TOTAL_FRAMES;
//  double led
//
//
//  
//  int highBoundsMapValue =map(ledNo, wavePosition, wavePosition + waveWidth,-128,128);
//  int lowBoundsMapValue =map(ledNo+1, wavePosition, wavePosition + waveWidth,-128,128);
//  int nextWavePositionFrame = map(wavePosition+1,stairs[stair].firstIndex-PULSE_STAIRS_WAVE_WIDTH,stairs[stair].lastIndex,0, PULSE_STAIRS_TOTAL_FRAMES);
//  int currentWavePositionFrame = map(wavePosition,stairs[stair].firstIndex-PULSE_STAIRS_WAVE_WIDTH,stairs[stair].lastIndex,0, PULSE_STAIRS_TOTAL_FRAMES);
//  //int upperBound = (PULSE_STAIRS_TOTAL_FRAMES/(stairs[stair].lastIndex-(stairs[stair].firstIndex)+PULSE_STAIRS_WAVE_WIDTH+1))+1;
//  int upperBound = nextWavePositionFrame - currentWavePositionFrame;
//  int mapValue =map(pulseStairsFrameNumber%upperBound,
//  0, 
//  (PULSE_STAIRS_TOTAL_FRAMES/(stairs[stair].lastIndex-(stairs[stair].firstIndex)+PULSE_STAIRS_WAVE_WIDTH+1))+2,
//  lowBoundsMapValue,
//  highBoundsMapValue);
//  int cosValue = cos8(mapValue);
//  #ifdef DEBUG5
//    if (stair == 0) {
//      Serial.print("Mapval input: ");
//      Serial.print(pulseStairsFrameNumber%upperBound);
//      Serial.print(" input up: ");
//      Serial.print(upperBound);
//      Serial.print(" lowmapValue: ");
//      Serial.print(lowBoundsMapValue);
//      Serial.print(" highmapValue: ");
//      Serial.print(highBoundsMapValue);
//      Serial.print(" mapValue: ");
//      Serial.print(mapValue);
//      Serial.print(" cosValue: ");
//      Serial.println(cosValue);
//    }
//  #endif
//  double factor = (double)cosValue/255.0;
//  return (int)((input * factor)+0.5);
//}
//
////end of pulse stairs
//
////global variables for stairID
//unsigned long stairIDLastFrame = 0;
//unsigned long stairIDLastStair = 0;
////illuminate stairs one by one
//void stairID() {
//  byte secondsPerStair = 1;
//  if ((millis() - stairIDLastFrame) > secondsPerStair*1000) {
//    stairIDLastFrame = millis();
//    Serial.print("Showing stair number ");
//    if (stairIDLastStair >= NUMBER_OF_STAIRS) {
//      for (int i = stairs[NUMBER_OF_STAIRS-1].firstIndex; i <= stairs[NUMBER_OF_STAIRS-1].lastIndex; i++) {
//        leds[i].setRGB(0,0,0);
//      }
//      
//      stairIDLastStair = 0;
//    }
//    Serial.println(stairIDLastStair);
//    if (stairIDLastStair > 0) {
//      for (int i = stairs[stairIDLastStair-1].firstIndex; i <= stairs[stairIDLastStair-1].lastIndex; i++) {
//        leds[i].setRGB(0,0,0);
//      }
//    }
//    for (int i = stairs[stairIDLastStair].firstIndex; i <= stairs[stairIDLastStair].lastIndex; i++) {
//      leds[i].setRGB(color[0],color[1],color[2]);
//    }
//    stairIDLastStair++;  
//  }
//}
//
//double mapf(double x, double in_min, double in_max, double out_min, double out_max)
//{
//    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//}

