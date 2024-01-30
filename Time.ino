// There's a hard number on how many alarms can be set at the same time.
// Set it on TimeAlarms.h
int halfHoursLeft = PHOTOPERIODIC_HOURS*2;

void setAlarms() {
  // lights on 10 minutes every 20 minutes for 4 hours
  // Minus one hour
  // Sunsets in CDMX
  // 19:08:27 on 03
  // 19:17:06 on 04
  // 19:28:41 on 05
  // 19:40:48 on 06
  // 19:26:30 on 08
  // water on every three (?) days at 8am

  // set water on alarms
  //Alarm.alarmRepeat(dowMonday, WATERHOUR, WATERMINUTE, 01, turnOnWaterAlarm);    // 8:30:30 every Saturday
  Alarm.alarmRepeat(dowTuesday, WATERHOUR, WATERMINUTE, 01, turnOnWaterAlarm);  // 8:30:30 every Saturday
  /* Alarm.alarmRepeat(dowSaturday, WATERHOUR, WATERMINUTE, 01, turnOnWaterAlarm);  // 8:30:30 every Saturday */

  // Supplemental lightning for providing a bit more of growth
  Alarm.alarmRepeat(LIGHTONHOUR, LIGHTONMINUTE, 00, turnOnLights);    // 8:30:30 every Saturday
  Alarm.alarmRepeat(LIGHTOFFHOUR, LIGHTOFFMINUTE, 00, turnOffLights);  // 8:30:30 every Saturday

  // Photoperiodic lightning to cheaply increase day length.
  Alarm.alarmRepeat(LIGHTOFFHOUR, LIGHTOFFMINUTE, 05, beginLightSequence);

}

void turnOnWaterAlarm(){
  // Instead of setting two alarms, set one and a timer.
  Serial.println("Turning scheduled water on.");
  turnOnWater();
  Alarm.timerOnce(60*WATER_INTERVAL, turnOffWater);
}


void beginLightSequence(){
  // The hours need to be doubled since our cycle of turning lights on and off takes thirty minutes.
  halfHoursLeft = PHOTOPERIODIC_HOURS*2;
  Serial.println("Start Light Sequence for " + String(halfHoursLeft) + " 30 minute segments");
  lightBlock();
}

void lightBlock(){
  // Begins a light sequence during N half hours
  if(halfHoursLeft != 0){
    Serial.println("Begin lightblock #" + String(halfHoursLeft));
    Alarm.timerOnce(60*30, lightBlock);
    photoLightBlock();
    halfHoursLeft--;
  }
}

void photoLightBlock(){
  // lights on 10 minutes every 20 minutes
  turnOnLights();
  // delay 10 minutes and turn lights off
  Alarm.timerOnce(60*10, turnOffLights);
  // It's not needed to delay another 20 minutes, since this will be called again by then.
}

String getTimeFromSerial() {
  // send a TIME message into serial.
  // ideally, we will receive a string, if not, TODO
  int attempts = 0;
  while (attempts < 10) {
    Serial.write("TIME\n");
    /* if(DEBUG){ */
    /*   delay(10); */
    /* } */
    String time = Serial.readString();
    if (!time.startsWith("TIME")) {
      debugPrint("Failed to read time. Retrying...");
      attempts++;
      continue;
    }
    // receive time in the shape, separated by :
    // TIME:YYYY:MM:DD:hh:mm:ss
    int tagsep = time.indexOf(":");
    debugPrint("Got time " + time.substring(tagsep+1));
    return time.substring(tagsep+1);
  }
  debugPrint("Max time setting attempts reached. Returning 0...");
  return "0";
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss) {
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet);  //convert to time_t
}

time_t getTime_TFromSerial() {
  String time = getTimeFromSerial();
  if (time.equals("0")) {
    debugPrint("Could not set time from sync job.");
    return 0;
  }
  int year = time.substring(0, 4).toInt();
  byte month = (byte)time.substring(5, 7).toInt();
  byte day = (byte)time.substring(8, 10).toInt();
  byte hour = (byte)time.substring(11, 13).toInt();
  byte minute = (byte)time.substring(14, 16).toInt();
  byte second = (byte)time.substring(17).toInt();
  return tmConvert_t(year, month, day, hour, minute, second);
}

void beginTime() {
  debugPrint("Setting time for the first time:");
  // Call getTimeFromSerial every 5 minutes to sync time
  debugPrint("Setting alarm timers.");
  if(DEBUG){
    setTime(6,59,55,7,2,2022);
  }
  else{
    //setTime(LIGHTOFFHOUR,LIGHTOFFMINUTE,0,7,2,2022);
    setTime(getTime_TFromSerial());
    setSyncProvider(getTime_TFromSerial);
    setSyncInterval(60 * TIMESYNC_INTERVAL);
  }

}
