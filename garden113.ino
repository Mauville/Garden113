/***
  This sketch works by setting flags at the appropiate times.
  On every loop, the arduino will check if the flags are on, and send output to the pins if they are.
  The flags are usually toggled using the TimeAlarm function, but manual override is possible.
  The arduino will listen on serial for specific codes (see Comms.ino/handleMessage), that trigger the flags.

  ***/
#include <Time.h>
#include <TimeAlarms.h>

#define DEBUG false
#define WATER_INTERVAL 5 // in minutes (max 29)
#define SLOW 0
#define STEADY 1
#define BLINKSPEED_SLOW 500
#define BLINKSPEED_STEADY 1000

#define LIGHTPIN 2
#define WATERPIN 10

// Incoming serial message
bool lights = false;
bool water = false;
bool manual = false;

void setup() {
  Serial.begin(9600);
  // setup flashing onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  // pin d2 for lights
  pinMode(LIGHTPIN, OUTPUT);
  // pin d10 for water
  pinMode(WATERPIN, OUTPUT);
  setTime(0,00,00,1,1,22);
  // set alarms to turn flags on and off
  setAlarms();
}

void loop() {
  // Print Status line
  Alarm.delay(1);
  blink(SLOW);
  printStatus();
  if (!manual) {
    autoRoutine();
  }
}


void turnOnLights() {
  debugPrint("Turned Lights ON");
  lights = true;
}
void turnOffLights() {
  debugPrint("Turned Lights OFF");
  lights = false;
}
void turnOnWater() {
  debugPrint("Turned Water ON");
  water = true;
}
void turnOffWater() {
  debugPrint("Turned Water OFF");
  water = false;
}


bool autoRoutine() {
  // This is a function checked on every second.
  // Since we don't have threads, this function needs to execute for as short as possible to avoid locking up the program
  if(lights){
    digitalWrite(LIGHTPIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(LIGHTPIN, LOW);  // turn the LED on (HIGH is the voltage level)
  }

  if(water){
    digitalWrite(WATERPIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(WATERPIN, LOW);  // turn the LED on (HIGH is the voltage level)
  }
  return false;
}
