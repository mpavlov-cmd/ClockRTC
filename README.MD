# Clock project for Arduino 

Arduino On Breadboard: https://electronoobs.com/eng_arduino_tut33.php
Custom LCD Chars: https://maxpromer.github.io/LCD-Character-Creator/
Low Power: https://www.gammon.com.au/power
Interrupts: https://www.youtube.com/watch?v=wIcC8-g9Lnw
Pointers C++: https://www.youtube.com/watch?v=jdQaaFNpM_4

## TODO List
- [x] Add limits for time and and dates
- [x] Rewrite app to object model for ease of extention
- [x] Manage LCD backlight - turn on for x seconds when any button is clicked
- [x] Play sound when button is clicked
- [x] Add alarm feature
- [x] Implement showing time without seconds
- [ ] Go to sleep mode after X minutes of inactivity. In sleep mode time is refreshed based on the watch-dog interrupt, time shown without seconds. Any button click would run interrupt to exit sleep mode, and set sleep mode timer to 0;
- [ ] Add HUD in 2 modes
- [ ] Actually fire alarm with beeper
- [ ] Display day of week

Optionally: 
- [ ] Add timer