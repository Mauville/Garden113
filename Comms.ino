String message;

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
  } else if (message.startsWith("ECHO:")) {
    Serial.println("ECHO: " + message + " from Arduino!");
    /* blink(STEADY); */
  } else if (message.startsWith("STATUS")) {
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
    if (a.equals("0")) {
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
