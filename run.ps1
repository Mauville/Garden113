arduino-cli.exe compile -p COM5 .\garden113.ino --fqbn arduino:avr:uno && `
arduino-cli.exe upload -p COM5 .\garden113.ino --fqbn arduino:avr:uno
# python garden113.tests.py
