# heatbox
LiPo heatbox controller for 12v heatmats using PI control loop and 16x2 LCD

# features
* PI control loop for exact and fast temperature control
* 16x2 lcd for data visualisation and configuration
* 4 buttons (up / down / enter / return) for easy navigation and configuration
* max. current protection
* min. voltage protection
* max temperature protection
* 2 profiles with different settings (e.g. one for heating at home with stationary supply and one in car with different min. voltage protection)

# hardware
the lcd is connected using i2c, standard 16x2 hd44780 lcd.

the current is measured using an acs712 current sensor with 5amps.

you need a voltage divider on the voltage pin.

## pins
|pin|description|
|---|---|
|A0|voltage measurement|
|A2|current measurement|
|4|eeprom reset|
|5|ground for buttons|
|6|enter button|
|7|up button|
|8|down button|
|9|return button|
|10|mosfet for heat mat|
|12|onewire sensors|
|i2c|lcd|

# libraries used
https://github.com/PaulStoffregen/OneWire

https://github.com/milesburton/Arduino-Temperature-Control-Library
