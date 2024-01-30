/***
  This sketch works by setting flags at the appropiate times.
  On every loop, the arduino will check if the flags are on, and send output to the pins if they are.
  The flags are usually toggled using the TimeAlarm function, but manual override is possible.
  The arduino will listen on serial for specific codes (see Comms.ino/handleMessage), that trigger the flags.

  ***/
#include <Time.h>
#include <TimeAlarms.h>
#include <RtcDS1302.h>

#define DEBUG true
#define RTC_TIME_ENABLED true


#define SLOW 0
#define STEADY 1
#define BLINKSPEED_SLOW 500
#define BLINKSPEED_STEADY 1000
#define TIMESYNC_INTERVAL 43200 // a month in minutes

#define LIGHTPIN 2
#define WATERPIN 3
#define RTC_CLK 9
#define RTC_DAT 8
#define RTC_RES 7

// Whether to water every N days, or a set of defined days at certain times
#define WATER_ON_INTERVALS true
#define WATER_EVERY_N_DAYS 4
// Whether to spread out watering even more on rain season
#define SUMMER_WATER_ADJUST true
#define WATERHOUR 8
#define WATERMINUTE 30
#define WATER_INTERVAL 12  // in minutes (max 29)
#define LIGHTONHOUR 18
#define LIGHTONMINUTE 30
#define LIGHTOFFHOUR 20
#define LIGHTOFFMINUTE 00

#define PHOTOPERIODIC_HOURS 4

ThreeWire RTCWire(RTC_DAT,RTC_CLK,RTC_RES); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(RTCWire);


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
  Rtc.Begin();
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
  // for some reason, the states are inverted in the relays

  if (lights) {
    digitalWrite(LIGHTPIN, LOW);
  } else {
    digitalWrite(LIGHTPIN, HIGH);
  }

  if (water) {
    digitalWrite(WATERPIN, LOW);
  } else {
    digitalWrite(WATERPIN, HIGH);
  }
  return false;
}
