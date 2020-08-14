//code pertaining to driving the IR Receiver to get commands via IR remote

#include <IRremote.h>

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

void turnOff() {
      systemOn = false;
      color[0] = 0;
      color[1] = 0;
      color[2] = 0;
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
