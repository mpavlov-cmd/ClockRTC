# Clock project for Arduino 

Arduino On Breadboard: https://electronoobs.com/eng_arduino_tut33.php
Custom LCD Chars: https://maxpromer.github.io/LCD-Character-Creator/
Low Power: https://www.gammon.com.au/power
Interrupts: https://www.youtube.com/watch?v=wIcC8-g9Lnw
More Interruupts: https://dronebotworkshop.com/interrupts/
Pointers C++: https://www.youtube.com/watch?v=jdQaaFNpM_4
More on Wath Dog Interrupts: https://forum.arduino.cc/t/watchdog-timer-interrupt-and-external-interrupt/194488
Cheap PCB Assebmly: https://jlcpcb.com/

Motion Sensor: SR602 
TCA4307 Hot-Swappable I2C Bus Buffer

## TODO List
- [x] Add limits for time and and dates
- [x] Rewrite app to object model for ease of extention
- [x] Manage LCD backlight - turn on for x seconds when any button is clicked
- [x] Play sound when button is clicked
- [x] Add alarm feature
- [x] Implement showing time without seconds
- [x] Go to sleep mode after X minutes of inactivity. In sleep mode time is refreshed based on the watch-dog interrupt, time shown without seconds. Any button click would run interrupt to exit sleep mode, and set sleep mode timer to 0; 
- [x] Disable ADC
- [x] Configure clock only once to run from battery and maintain configuration
- [x] Add HUD in 2 modes
- [x] Refactor and rename clock modes, so there is a dedicated parent class for editable mode
- [ ] Actually fire alarm with beeper, in case interrupted - turn off
- [ ] Add settings screen: Deep Sleep, Backlight Duration, Sound
- [ ] Display day of week

Optionally: 
- [ ] Add timer