#include <Time.h>
#include <TimeAlarms.h>

#define DEBUG false
#define WATER_INTERVAL 5 // in minutes (max 29)
#define SLOW 0
#define STEADY 1
#define BLINKSPEED_SLOW 500
#define BLINKSPEED_STEADY 1000

// Incoming serial message
String message;
bool lights = false;
bool water = false;
bool manual = false;

void setup() {
  Serial.begin(9600);
  // Flash onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  setTime(0,00,00,1,1,22); // set time to Saturday 8:29:00am Jan 1 2011
  // set water on alarms
  Alarm.alarmRepeat(dowMonday,8,30,00,turnOnWater);  // 8:30:30 every Saturday 
  Alarm.alarmRepeat(dowThursday,8,30,00,turnOnWater);  // 8:30:30 every Saturday 
  Alarm.alarmRepeat(dowSaturday,8,30,00,turnOnWater);  // 8:30:30 every Saturday 
  // set water off alarms
  Alarm.alarmRepeat(dowMonday,8,30 + WATER_INTERVAL,00,turnOffWater);  // 8:30:30 every Saturday 
  Alarm.alarmRepeat(dowThursday,8,30 + WATER_INTERVAL,00,turnOffWater);  // 8:30:30 every Saturday 
  Alarm.alarmRepeat(dowSaturday,8,30 + WATER_INTERVAL,00,turnOffWater);  // 8:30:30 every Saturday 
  // set light alarm
  Alarm.alarmRepeat(18,00,0, turnOnLights);  // 8:30am every day
  Alarm.alarmRepeat(01,00,0, turnOffLights);  // 1:00am every day
}

void loop() {
  // Print Status line
  blink(SLOW);
  printStatus();
  if (!manual) {
    autoRoutine();
  }
}

// This procedure gets called every time something is received on Serial
void serialEvent() {
  // Read and echo back messages
  message = "";
  message = Serial.readString();
  // Handle COMMANDs
  if (message.startsWith("COMMAND:")) {
    message = message.substring(8);
    Serial.println("ACK:" + message);
    /* blink(STEADY); */
    // Pass sliced string
    handleCommand(message);
    Serial.println("RESPONSE:1");
  } else if (message.startsWith("ECHO:")){
    Serial.println("ECHO: " + message + " from Arduino!");
    /* blink(STEADY); */
  } else if (message.startsWith("STATUS")){
    printStatus();
    /* blink(SLOW); */
  }
}

bool handleCommand(String command) {
  // parse a response of the shape MANUAL LIGHTS WATER.
  // This response will be:
  // 0  turn off
  debugPrint("Received " + command);
  String a = String(command.charAt(0));
  String l = String(command.charAt(1));
  String w = String(command.charAt(2));
  if (manual) {
    // Turn auto back on.
    if (a.equals("0")){
      manual = false;
      turnOffLights();
      turnOffWater();
      return true;
    }
    debugPrint("Executing manual commands");
    if (l.equals("1")) {
      turnOnLights();
    }
    if (l.equals("0")) {
      turnOffLights();
    }
    if (w.equals("1")) {
      turnOnWater();
    }
    if (w.equals("0")) {
      turnOffWater();
    }
  } else {
    // Ignore every command on auto except turning to manual.
    if (a.equals("1")) {
      debugPrint("Manual Turned ON");
      manual = true;
      return handleCommand(command);
    }
  }
}

void debugPrint(String s) {
  if (DEBUG)
    Serial.println("DEBUG: " + s);
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
  return false;
  // in here go auto things
  // This is a function checked on every second.
  // Since we don't have threads, this function needs to execute for as short as possible to avoid locking up the program
  // perhaps only ifs that turn on and off when time is inside a range
  // lights on 1 hour from sunset
  // 19:08:27 on 03
  // 19:17:06 on 04
  // 19:28:41 on 05
  // 19:40:48 on 06
  // 19:26:30 on 08
  // lights on 10 minutes every 20 minutes for 4 hours

  // water on every three (?) days at 8am
}
void printStatus() {
  Serial.print("STATUS:");
  Serial.print(String(manual));
  Serial.print(String(lights));
  Serial.print(String(water));
  Serial.println();
}
void blink(int speed) {
  (speed == SLOW) ? speed = BLINKSPEED_SLOW : speed = BLINKSPEED_STEADY;
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(speed);                     // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(speed);
}
