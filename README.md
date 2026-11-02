# Halloween LED Costume

An Arduino Uno R4 minima costume-lighting project with 29 addressable LEDs,
multiple animated effects and a few controls added onto it

## Hardware

- Arduino Uno R4 minima
- 29 HD107S addressable LEDs
- Push button connected to pin 12 and ground
- Potentiometer on pin A0
- 
- LED data on pin 11 and clock on pin 13

This amount of LED's it seems to be runnin of the 5v pin of the arduino uno, but it really shouldnt
Add a external power supply to the led strip, connect the grounds of the uno and the power supply.

## Effects

The firmware cycles through solid purple, color wipe, chase, twinkle, breathing,
meteor, fire, palette, and rainbow effects.

## Build and upload

Install [PlatformIO Core](https://platformio.org/install/cli), connect the board,
then run these commands from the project directory:

```sh
pio run
pio run --target upload
```      

FastLED is installed automatically from the dependency declared in
`platformio.ini`.
    
