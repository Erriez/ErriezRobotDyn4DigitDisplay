#!/bin/sh

# Install Python platformio 
# pip install -U platformio

# Update library
# git fetch
# git pull

# Build example(s)
platformio lib install https://github.com/Erriez/ErriezRobotDyn4DigitDisplay.git
platformio lib install https://github.com/Erriez/Erriez1637.git
platformio lib update
platformio ci --lib=".piolibdeps/Erriez RobotDyn 4-digit display" --lib=".piolibdeps/Erriez TM1637" --board uno --board micro --board d1_mini --board nanoatmega328 --board pro16MHzatmega328 --board pro8MHzatmega328 --board leonardo --board megaatmega2560 --board d1_mini --board nodemcuv2 ../examples/7SegementDisplayDemo/7SegementDisplayDemo.ino
