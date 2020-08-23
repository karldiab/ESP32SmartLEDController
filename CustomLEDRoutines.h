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
#define NUMBER_OF_STAIRS 12
const staircaseStep  stairs[NUMBER_OF_STAIRS] PROGMEM = {
  {0,10,false},
  {11,22,true},
  {23,34,false},
  {35,46,true},
  {47,58,false},
  {59,70,true},
  {71,82,false},
  {83,94,true},
  {95,106,false},
  {107,118,false},
  {119,154,false},
  {155,191,false}
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
int pulseStairsFrameNumber = 0;
#define PULSE_STAIRS_TOTAL_FRAMES 255
#define PULSE_STAIRS_WAVE_WIDTH 6
//double calculateCosWaveformFactor(int ledNo, int wavePosition, int stair);
double calculateCosWaveformFactor(int ledNo, int wavePosition, int frameNumber, int totalFrames, int waveWidth, int stair);
double mapf(double x, double in_min, double in_max, double out_min, double out_max);
//lights up all the stairs at the same time by pulsing out light from the center
void pulseStairs() {
  int wavePosition;
  if (pulseStairsFrameNumber > PULSE_STAIRS_TOTAL_FRAMES) {
    pulseStairsFrameNumber = 0;
  }
  #ifdef DEBUG5
    Serial.print("Frame number: ");
    Serial.println(pulseStairsFrameNumber);
  #endif
  for (int stair = 0; stair < NUMBER_OF_STAIRS; stair++) {
    //calculate where the start of the wave should
    wavePosition = map(pulseStairsFrameNumber, 0, PULSE_STAIRS_TOTAL_FRAMES, stairs[stair].firstIndex-PULSE_STAIRS_WAVE_WIDTH,stairs[stair].lastIndex);
    double waveProgress = (double)pulseStairsFrameNumber/PULSE_STAIRS_TOTAL_FRAMES;
    //wavePosition = getWavePosition();
    #ifdef DEBUG5
      if (stair == 0) {
        Serial.print("wavePosition: ");
        Serial.println(wavePosition);
      }

    #endif
    for (int led = stairs[stair].firstIndex; led <= stairs[stair].lastIndex; led++) {
      int myLEDIndex = stairs[stair].orderReversed ? map(led,stairs[stair].firstIndex,stairs[stair].lastIndex,stairs[stair].lastIndex,stairs[stair].firstIndex) : led;
      //check if this LED is one of the ones that should be illuminated
      if (led >= wavePosition +1 && led < wavePosition + PULSE_STAIRS_WAVE_WIDTH + 1) {

        //int ledBrightness = 
        //double brightnessFactor = mapf(color
        double brightnessFactor = calculateCosWaveformFactor(led,wavePosition, pulseStairsFrameNumber, PULSE_STAIRS_TOTAL_FRAMES, PULSE_STAIRS_WAVE_WIDTH, stair);
        leds[myLEDIndex].setRGB((int)((double)color[0]*brightnessFactor+0.5),(int)((double)color[1]*brightnessFactor+0.5),(int)((double)color[2]*brightnessFactor+0.5)); 
      #ifdef DEBUG5
        if (stair == 1 || stair == 0) {
          Serial.print("stair: ");
          Serial.print(stair);
          Serial.print(" myLEDIndex: ");
          Serial.print(myLEDIndex);
          Serial.print(" wavePosition: ");
          Serial.print(wavePosition);
          Serial.print(" brightnessFactor: ");
          Serial.println(brightnessFactor);
        }
      #endif
      } else {
        //if its not, set to black;
        
        leds[myLEDIndex].setRGB(0,0,0);
      }
    }
    
  }
  
  #ifdef DEBUG5
  
    Serial.println();
  #endif
  pulseStairsFrameNumber++;
}

double calculateCosWaveformFactor(int ledNo, int wavePosition, int frameNumber, int totalFrames, int waveWidth, int stair) {
  double waveProgress = (double)frameNumber/totalFrames;
  double waveProgressLEDNo = mapf(waveProgress, 0,1,(double)stairs[stair].firstIndex-waveWidth,stairs[stair].lastIndex);
  double ledCosInput = mapf((double)ledNo, waveProgressLEDNo, waveProgressLEDNo+waveWidth,(double)-128,(double)128);
  int ledCosValue = cos8((int)(ledCosInput+0.5));
  #ifdef DEBUG6
    if (stair == 0) {
      Serial.print("ledNo: ");
      Serial.print(ledNo);
      Serial.print(" waveProgress: ");
      Serial.print(waveProgress);
      Serial.print(" waveProgressLEDNo: ");
      Serial.print(waveProgressLEDNo);
      Serial.print(" ledCosInput: ");
      Serial.print(ledCosInput);
      Serial.print(" ledCosValue: ");
      Serial.println(ledCosValue);
    }
  #endif
  return (double)ledCosValue/255.0;
}

//end of pulse stairs

//red carpet
int redCarpetFrameNumber = 0;
int redCarpetStairNumber = 0;
bool reverseDirection = false;
#define RED_CARPET_TOTAL_FRAMES 24
#define RED_CARPET_WAVE_WIDTH 12
void redCarpetLoop() {
  int wavePosition;
  if (redCarpetStairNumber == 0 && redCarpetFrameNumber == 0) {
    for(int i=0; i<LED_COUNT; i++) { 
      leds[i].setRGB(0,0,0);
    }
  }
  if (redCarpetFrameNumber > RED_CARPET_TOTAL_FRAMES) {
    redCarpetFrameNumber = 0;
    if (!reverseDirection) {
      redCarpetStairNumber++;
    } else {
      redCarpetStairNumber--;
    }
    
  }
  if (redCarpetStairNumber >= NUMBER_OF_STAIRS && !reverseDirection) {
    redCarpetStairNumber -= 1;
    reverseDirection = !reverseDirection;
  }
  if (redCarpetStairNumber < 0 && reverseDirection) {
    redCarpetStairNumber = 0;
    reverseDirection = !reverseDirection;
  }
  #ifdef DEBUG5
    Serial.print("redCarpetStairNumber: ");
    Serial.print(redCarpetStairNumber);
    Serial.print(" Frame number: ");
    Serial.println(redCarpetFrameNumber);
  #endif
  int stair = redCarpetStairNumber;
  //calculate where the start of the wave should
  wavePosition = map(redCarpetFrameNumber, 0, RED_CARPET_TOTAL_FRAMES, stairs[stair].firstIndex-RED_CARPET_WAVE_WIDTH,stairs[stair].lastIndex);
  double waveProgress = (double)redCarpetFrameNumber/RED_CARPET_TOTAL_FRAMES;
  //wavePosition = getWavePosition();
  #ifdef DEBUG5
    if (stair == 0) {
      Serial.print("wavePosition: ");
      Serial.println(wavePosition);
    }

  #endif
  for (int led = stairs[stair].firstIndex; led <= stairs[stair].lastIndex; led++) {
    //check if this LED is one of the ones that should be illuminated
    if (reverseDirection) {
      if (led >= wavePosition +1 && led < wavePosition + RED_CARPET_WAVE_WIDTH/2 + 3) {
        double brightnessFactor = calculateCosWaveformFactor(led,wavePosition, redCarpetFrameNumber, RED_CARPET_TOTAL_FRAMES, RED_CARPET_WAVE_WIDTH, stair);
        leds[led].setRGB((int)((double)color[0]*brightnessFactor+0.5),(int)((double)color[1]*brightnessFactor+0.5),(int)((double)color[2]*brightnessFactor+0.5)); 
      }
      if (led < wavePosition) {
        leds[led].setRGB(0,0,0);
      }
    } else {
      if (led >= wavePosition +1 + RED_CARPET_WAVE_WIDTH/2 && led < wavePosition + RED_CARPET_WAVE_WIDTH + 1) {
        double brightnessFactor = calculateCosWaveformFactor(led,wavePosition, redCarpetFrameNumber, RED_CARPET_TOTAL_FRAMES, RED_CARPET_WAVE_WIDTH, stair);
        leds[led].setRGB((int)((double)color[0]*brightnessFactor+0.5),(int)((double)color[1]*brightnessFactor+0.5),(int)((double)color[2]*brightnessFactor+0.5)); 
      }
    }
  }
  
  #ifdef DEBUG5
  
    Serial.println();
  #endif
  redCarpetFrameNumber++;
}
//return true if done
bool redCarpetUnroll() {
  int wavePosition;
  if (redCarpetStairNumber == 0 && redCarpetFrameNumber == 0) {
    for(int i=0; i<LED_COUNT; i++) { 
      leds[i].setRGB(0,0,0);
    }
  }
  
  if (redCarpetFrameNumber > RED_CARPET_TOTAL_FRAMES) {
    redCarpetFrameNumber = 0;
    redCarpetStairNumber++;
    
  }
  //only do first 10 stairs
  //when done make sure all leds on the stairs are fully lit
  if (redCarpetStairNumber >= 10) {
    for(int i=0; i<=stairs[9].lastIndex; i++) { 
      leds[i].setRGB(color[0],color[1],color[2]);
    }
    for(int i=stairs[9].lastIndex+1; i<=LED_COUNT; i++) { 
      leds[i].setRGB(0,0,0);
    }
    return true;
  }
  #ifdef DEBUG5
    Serial.print("redCarpetStairNumber: ");
    Serial.print(redCarpetStairNumber);
    Serial.print(" Frame number: ");
    Serial.println(redCarpetFrameNumber);
  #endif
  int stair = redCarpetStairNumber;
  //calculate where the start of the wave should
  wavePosition = map(redCarpetFrameNumber, 0, RED_CARPET_TOTAL_FRAMES, stairs[stair].firstIndex-RED_CARPET_WAVE_WIDTH,stairs[stair].lastIndex);
  double waveProgress = (double)redCarpetFrameNumber/RED_CARPET_TOTAL_FRAMES;
  //wavePosition = getWavePosition();
  #ifdef DEBUG5
    if (stair == 0) {
      Serial.print("wavePosition: ");
      Serial.println(wavePosition);
    }

  #endif
  for (int led = stairs[stair].firstIndex; led <= stairs[stair].lastIndex; led++) {
    //check if this LED is one of the ones that should be illuminated
    if (led >= wavePosition +1 + RED_CARPET_WAVE_WIDTH/2 && led < wavePosition + RED_CARPET_WAVE_WIDTH + 1) {
      double brightnessFactor = calculateCosWaveformFactor(led,wavePosition, redCarpetFrameNumber, RED_CARPET_TOTAL_FRAMES, RED_CARPET_WAVE_WIDTH, stair);
      leds[led].setRGB((int)((double)color[0]*brightnessFactor+0.5),(int)((double)color[1]*brightnessFactor+0.5),(int)((double)color[2]*brightnessFactor+0.5)); 
    }
    
  }
  
  #ifdef DEBUG5
  
    Serial.println();
  #endif
  redCarpetFrameNumber++;
  return false;
}
//return true if done
bool redCarpetRollUp() {
  int wavePosition;
  if (redCarpetFrameNumber > RED_CARPET_TOTAL_FRAMES) {
    redCarpetFrameNumber = 0;
    redCarpetStairNumber--;
    
  }
  //when done make sure all leds are off
  if (redCarpetStairNumber < 0) {
    for(int i=0; i<LED_COUNT; i++) { 
      leds[i].setRGB(0,0,0);
    }
    return true;
  }
  #ifdef DEBUG5
    Serial.print("redCarpetStairNumber: ");
    Serial.print(redCarpetStairNumber);
    Serial.print(" Frame number: ");
    Serial.println(redCarpetFrameNumber);
  #endif
  int stair = redCarpetStairNumber;
  //calculate where the start of the wave should
  wavePosition = map(redCarpetFrameNumber, 0, RED_CARPET_TOTAL_FRAMES, stairs[stair].firstIndex-RED_CARPET_WAVE_WIDTH,stairs[stair].lastIndex);
  double waveProgress = (double)redCarpetFrameNumber/RED_CARPET_TOTAL_FRAMES;
  //wavePosition = getWavePosition();
  #ifdef DEBUG5
    if (stair == 0) {
      Serial.print("wavePosition: ");
      Serial.println(wavePosition);
    }

  #endif
  for (int led = stairs[stair].firstIndex; led <= stairs[stair].lastIndex; led++) {
    if (led >= wavePosition +1 && led < wavePosition + RED_CARPET_WAVE_WIDTH/2 + 3) {
      double brightnessFactor = calculateCosWaveformFactor(led,wavePosition, redCarpetFrameNumber, RED_CARPET_TOTAL_FRAMES, RED_CARPET_WAVE_WIDTH, stair);
      leds[led].setRGB((int)((double)color[0]*brightnessFactor+0.5),(int)((double)color[1]*brightnessFactor+0.5),(int)((double)color[2]*brightnessFactor+0.5)); 
    }
    if (led < wavePosition) {
      leds[led].setRGB(0,0,0);
    }
  }
  
  #ifdef DEBUG5
  
    Serial.println();
  #endif
  redCarpetFrameNumber++;
  return false;
}


//end of red carpet

//global variables for stairID
unsigned long stairIDLastFrame = 0;
unsigned long stairIDLastStair = 0;
//illuminate stairs one by one
void stairID() {
  byte secondsPerStair = 1;
  if ((millis() - stairIDLastFrame) > secondsPerStair*1000) {
    stairIDLastFrame = millis();
    Serial.print("Showing stair number ");
    if (stairIDLastStair >= NUMBER_OF_STAIRS) {
      for (int i = stairs[NUMBER_OF_STAIRS-1].firstIndex; i <= stairs[NUMBER_OF_STAIRS-1].lastIndex; i++) {
        leds[i].setRGB(0,0,0);
      }
      
      stairIDLastStair = 0;
    }
    Serial.println(stairIDLastStair);
    if (stairIDLastStair > 0) {
      for (int i = stairs[stairIDLastStair-1].firstIndex; i <= stairs[stairIDLastStair-1].lastIndex; i++) {
        leds[i].setRGB(0,0,0);
      }
    }
    for (int i = stairs[stairIDLastStair].firstIndex; i <= stairs[stairIDLastStair].lastIndex; i++) {
      leds[i].setRGB(color[0],color[1],color[2]);
    }
    stairIDLastStair++;  
  }
}

double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

