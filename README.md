Trafo Pop
=========

wearable LEDs using Arduino Nano and WS2812B

This repositiory contains the software for the "build your own LED jacket" workshop.

http://www.trafopop.com/arduino-led-jacket-workshop

We have several branches, one for each jacket, coat, hat or bag:

- master
- cp
- mg
- pr
- sh
- ap
- mb
- db
- sk
- Handbag
…

Each branch is different because of different layout. It can also be different because of
different colors or scaling or different LEDs.
Please switch to your branch.

The code in this branch is used for the LEDer Handbag:
http://www.trafopop.com/project/everything-will-be-ok-bag/

---

Additional required tool for editing, compiling and uploading the code to the Arduino:
http://arduino.cc/en/Main/Software

If you connect your Arduino device to your computer you should see 
two additional serial ports in the Tools menu of the Arduino software. (on Mac)
If not please install the USB FTDI driver for the Arduino Nano from the FTDI website:
http://www.ftdichip.com/Drivers/VCP.htm

In the Arduino Software in the Tools menu select your Board: Arduino Nano w/ ATmega328
And select the serial port: /dev/tty.usbserial… (on Mac)

See also: http://arduino.cc/en/Guide/Howto

For using WS2812B LEDs which have a WS2811 chip build in and do not have the usual WS2801
chip an addition Arduino library has to be installed before you start your Arduino
software on your computer:
https://github.com/adafruit/Adafruit_NeoPixel

Additional optional tool for easier git usage:
http://www.sourcetreeapp.com
