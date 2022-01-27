/***
  This sketch works by setting flags at the appropiate times.
  On every loop, the arduino will check if the flags are on, and send output to the pins if they are.
  The flags are usually toggled using the TimeAlarm function, but manual override is possible.
  The arduino will listen on serial for specific codes (see Comms.ino/handleMessage), that trigger the flags.

  ***/
#include <Time.h>
#include <TimeAlarms.h>

#define DEBUG true

#define SLOW 0
#define STEADY 1
#define BLINKSPEED_SLOW 500
#define BLINKSPEED_STEADY 1000
#define TIMESYNC_INTERVAL 5 // in minutes TODO RESET TO 1

#define LIGHTPIN 2
#define WATERPIN 10

/* #define WATERHOUR 8 */
/* #define WATERMINUTE 30 */
/* #define LIGHTONHOUR 18 */
/* #define LIGHTONMINUTE 30 */
/* #define LIGHTOFFHOUR 1 */
/* #define LIGHTOFFMINUTE 00 */

#define WATERHOUR 0
#define WATERMINUTE 0
/* #define WATER_INTERVAL 5  // in minutes (max 29) */
#define WATER_INTERVAL 1  // in minutes (max 29)

#define LIGHTONHOUR 0
#define LIGHTONMINUTE 0
#define LIGHTOFFHOUR 0
#define LIGHTOFFMINUTE 1
#define PHOTOPERIODIC_HOURS 5

// Incoming serial message
bool lights = false;
bool water = false;
bool manual = false;

void printStatus() {
  Serial.print("STATUS:");
  Serial.print(String(manual));
  Serial.print(String(lights));
  Serial.print(String(water));
  if(DEBUG){
    Serial.print("|");
    Serial.print(String(year()));
    Serial.print(":");
    Serial.print(String(month()));
    Serial.print(":");
    Serial.print(String(day()) + "." +String(weekday()));
    Serial.print(":");
    Serial.print(String(hour()));
    Serial.print(":");
    Serial.print(String(minute()));
    Serial.print(":");
    Serial.print(String(second()));
  }
  if (timeStatus() != timeSet){
    Serial.print("|");
    Serial.print("TIMError:" + timeStatus());
  }
  Serial.println();

}
void blink(int speed) {
  (speed == SLOW) ? speed = BLINKSPEED_SLOW : speed = BLINKSPEED_STEADY;
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  Alarm.delay(speed);                     // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  Alarm.delay(speed);
}
void debugPrint(String s) {
  if (DEBUG)
    Serial.println("DEBUG: " + s);
}


void setup() {
  Serial.begin(9600);
  // setup flashing onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LIGHTPIN, OUTPUT);
  pinMode(WATERPIN, OUTPUT);
  // set alarms to turn flags on and off
  beginTime();
  setAlarms();
}

void loop() {
  // Print Status line
  Alarm.delay(1000);
  printStatus();
  autoRoutine();
  /* blink(SLOW); */
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
  // It'll just turn on/off pins according to state. Other functions modify state.

  if (lights) {
    digitalWrite(LIGHTPIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  } else {
    digitalWrite(LIGHTPIN, LOW);  // turn the LED on (HIGH is the voltage level)
  }

  if (water) {
    digitalWrite(WATERPIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  } else {
    digitalWrite(WATERPIN, LOW);  // turn the LED on (HIGH is the voltage level)
  }
  return false;
}
