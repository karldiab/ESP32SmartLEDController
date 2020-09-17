#define DEBUG 1
#define DEBUG2 1
//#define DEBUG3 1
//#define DEBUG5 1
//#define WEB_SERVER 1
#define IR_RECEIVE_PIN 13
#define MOTION_PIN 27
//LED Strip driver definitions
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
#define LED_COUNT 191
#define LED_PIN    12
#define FRAMES_PER_SECOND  120
#define MAX_BRIGHTNESS_VALUE 10
#define ROUTINE_CYCLE_TIME_S 120
#define MS_NIGHT_MODE_STAY_ON 20000

//global variables to keep track of settings
enum displayMode {
  normal,
  night,
  off,
  wifi,
  remote,
  single
};
displayMode currentDisplayMode = normal;
displayMode previousDisplayMode = normal;
//volitile because it is accessed in interupt function
volatile bool motionDetected = false;
volatile unsigned long motionLastDetected = 0;
//volatile bool motionDetectBeingHandled = false;
//volatile bool carpetDoneUnroll = false;
//volatile bool carpetDoneRollup = false;
//brightness is a value from 0 to 10
volatile byte brightness = 10;
//array to store solid color to display on LEDs as commanded by the ir remote
int color[3] = {255,0,255}; //RGB values from 
bool colorManuallySelected = false;

#include "UtilityFunctions.h"
#include "LEDDriver.h"
#include "DisplayDriver.h"
#include "IRReceiver.h"
#ifdef WEB_SERVER
  #include "WebServer.h"
#endif


//create task to drive LEDs to run on second core
TaskHandle_t LEDTask;

//Hardware timer setup
hw_timer_t * myTimer = NULL; 
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//timer interupt function to check status of motion detector
void IRAM_ATTR ISR2() {
  //check if got new remote command
  if (irrecv.decode(&results)) {  // Grab an IR code
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
}



void setup(void) {
  Serial.begin(115200);
  //for motion detector
  pinMode(MOTION_PIN, INPUT);
//setup timer interupt firing every 150 ms to check status of motion detector
  myTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(myTimer, &ISR2, true);
  timerAlarmWrite(myTimer, 150000, true);
  timerAlarmEnable(myTimer);
  //for lcd
  setupDisplay();
  #ifdef WEB_SERVER
    webServerSetup();
  #endif


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
  delay(100); 
}
int counter = 0;
void loop() {
  //update display if its been long enough
  updateDisplay();
  #ifdef WEB_SERVER
    webServerLoop();
  #endif
  
}
