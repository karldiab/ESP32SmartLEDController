//code pertaining to driving the IR Receiver to get commands via IR remote

#include <IRremote.h>

IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;        // Somewhere to store the results
void decodeIRCommand(unsigned int value);

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

void toggleDisplayMode() {
  #ifdef DEBUG3
    Serial.println("Got remote command");
  #endif
   switch (currentDisplayMode) {
      case normal:
        previousDisplayMode = currentDisplayMode; 
        currentDisplayMode = single; 
      break;
      case single:
        previousDisplayMode = currentDisplayMode; 
        currentDisplayMode = normal; 
      break;
//      case night:
//        previousDisplayMode = currentDisplayMode; 
//        currentDisplayMode = normal; 
//      break;
      default:
        previousDisplayMode = currentDisplayMode; 
        currentDisplayMode = normal; 
      break;
    }
}

//ir receiver decode functions
void decodeIRCommand(unsigned int value) {
  switch (value) {
    case button_off:
      if (currentDisplayMode != off) { 
        previousDisplayMode = currentDisplayMode; 
        currentDisplayMode = off; 
      }
    break;
    case button_on:
      if (currentDisplayMode == off) { 
        currentDisplayMode = previousDisplayMode; 
      }
    break;
    //repurposed this button to change mode
    case button_flash:
      toggleDisplayMode();
    break;
    //repurposed this skip to next routine
    case button_strobe:
      nextPattern();
    break;
    //repurposed this button to go to previous routine
    case button_fade:
      previousPattern();
    break;
    case button_red:

      color[0] = 255;
      color[1] = 0;
      color[2] = 0;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_green:

      color[0] = 0;
      color[1] = 255;
      color[2] = 0;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_blue:
      
      color[0] = 0;
      color[1] = 0;
      color[2] = 255;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_white:
      
      color[0] = 255;
      color[1] = 255;
      color[2] = 255;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_brightnessUp:
      
      if (brightness < MAX_BRIGHTNESS_VALUE)
        brightness++;
      changeBrightness();
      
    break;
    case button_brightnessDown:
      
      if (brightness > 0)
        brightness--;
      changeBrightness();
    break;
    case button_orange:
      
      color[0] = 255;
      color[1] = 60;
      color[2] = 0;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_aquamarine:
      
      color[0] = 32;
      color[1] = 210;
      color[2] = 137;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_navy:
      
      color[0] = 0;
      color[1] = 0;
      color[2] = 128;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_darkOrange:
      
      color[0] = 255;
      color[1] = 45;
      color[2] = 0;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_turquoise:
      
      color[0] = 64;
      color[1] = 224;
      color[2] = 208;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_darkPurple:
      
      color[0] = 128;
      color[1] = 0;
      color[2] = 128;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_brightOrange:
      
      color[0] = 255;
      color[1] = 100;
      color[2] = 0;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_mediumTurquoise:
      
      color[0] = 72;
      color[1] = 209;
      color[2] = 204;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_purple:
      
      color[0] = 128;
      color[1] = 0;
      color[2] = 156;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_yellow:
      
      color[0] = 255;
      color[1] = 165;
      color[2] = 0;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_darkTurquoise:
      
      color[0] = 0;
      color[1] = 206;
      color[2] = 209;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    case button_pink:
      
      color[0] = 255;
      color[1] = 20;
      color[2] = 147;
      if (currentDisplayMode != remote) { previousDisplayMode = currentDisplayMode; currentDisplayMode = remote; }
    break;
    default:
      return;
    break;
  }
}
