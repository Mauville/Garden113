import serial
import time
import threading

DEBUG = False

def poll():
    global passed
    current = ""
    while(True):
        line =ser.readline().decode("ASCII")
        if line and line != current:
            if line.startswith("STATUS"):
                current = line
            if not DEBUG:
                if line.startswith("STATUS") or line.startswith("ACK"):
                    print(line[:-1])
            else:
                print(line[:-1])





expected = ""
ser = serial.Serial('COM6', 9600, timeout=1)  # open serial port
# Start reading in background
poller = threading.Thread(target=poll)
poller.daemon = True
poller.start()

print("== Expect same as ACK")
ser.write(b"COMMAND:111")
ser.write(b"STATUS")
time.sleep(3)
ser.write(b"COMMAND:100")
expected = "100"
time.sleep(5)
ser.write(b"COMMAND:110")
expected = "110"
time.sleep(5)
ser.write(b"COMMAND:101")
expected = "101"
time.sleep(5)

print("==Expect 000")
ser.write(b"COMMAND:011")
expected = "000"
time.sleep(5)
ser.write(b"COMMAND:001")
expected = "000"
time.sleep(5)

print("Writing hello")
ser.write(b"Hello")

exit(0)


