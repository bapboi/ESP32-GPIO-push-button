# ESP32-GPIO-push-button

## emulates a momentary switch with a web server hosted from an esp 32

this program currently replaces a push button on a Hunter fan remote by introducing a LOW path from HIGH, because building a website and buying a relay/rf transmitter/capture unit was more expensive than just wiring to the switch and shorting it.

# instructions

in the same directory include "wificonfig.h", where you define \
`WIFI_SSID = your ssid`\
`WIFI_PASSWORD = your password`\

on the esp32-wroom-32, GPIO pins 25 and 26 are used. configure according to your esp32 model and which pins are available, and ensure a path to ground is still available from the esp32 to whatever environment you are using.

required is a method to upload to binary to the ESP32, whether through the Arduino IDE, or using arduino-cli and esptool.py

i didn't do the latter because i didn't want to

this generally isn't useful to anyone except someone who doesn't want to go out of their way to actually get proper tools for capturing and resending a 433mhz signal, or better yet using a relay module
