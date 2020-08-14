#define DEBUG 1
#define IR_RECEIVE_PIN 13
#define LED_PIN    12
#define MOTION_PIN 27
#define LED_COUNT 100

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
#include "DisplayDriver.h"
#include "IRReceiver.h"
#include "LEDDriver.h"


//create task to drive LEDs to run on second core
TaskHandle_t LEDTask;

//Hardware timer setup
hw_timer_t * myTimer = NULL; 
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

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
  setupDisplay();


  //ir receiver
  irrecv.enableIRIn();  // Start the receiver
  
  //for LED controls
  setupLEDs();
  xTaskCreatePinnedToCore(
                    LEDTaskCode,   /* Task function. */
                    "LEDTask",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &LEDTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 
}
void loop() {
  //updateDisplay();

    
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
