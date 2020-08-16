/*These are LED routines tailored to my custom LED layout, ie deployed on my spiral staircase and around my glass fixtures
 * The LED numbers and organization are hardcoded according to my system. These routines may work for you, but probably
 * won't look good because you'll have a different layout
 */

 // progmem variables to organize my own LED layout
struct staircaseStep {
  uint8_t firstIndex; //first index, inclusive
  uint8_t lastIndex; //last index, inclusive
  bool orderReversed; //do the leds run backwards?
};
#define NUMBER_OF_STEPS 9
const staircaseStep  stairs[NUMBER_OF_STEPS] PROGMEM = {
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


//global variables for stairID
unsigned long stairIDLastFrame = 0;
unsigned long stairIDLastStair = 0;
//illuminate stairs one by one
void stairID() {
  byte secondsPerStair = 1;
  if ((millis() - stairIDLastFrame) > secondsPerStair*1000) {
    stairIDLastFrame = millis();
    Serial.print("Showing stair number ");
    if (stairIDLastStair >= NUMBER_OF_STEPS) {
      for (int i = stairs[NUMBER_OF_STEPS-1].firstIndex; i <= stairs[NUMBER_OF_STEPS-1].lastIndex; i++) {
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
