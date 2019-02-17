## ESP8266 based window blind control

### Hardware

I am using a Sonoff DUAL R2. Two relays are needed to control the motor rotation
direction.

The first FW flash needs to be done thru UART, shorting BUTTON 0 to GND.
Subsequent FW updates can be done via OTA.

### Screenshot (mobile website)

<img src="https://raw.githubusercontent.com/marcov/esp-blindcontrol/master/screenshot.png" height="600px">


### Push Button behaviour

- Short press: move up or down the blind (for debug purposes). Up / down direction
is toggled at each button press.
- Long press (at least 6 seconds): reboot the module.
