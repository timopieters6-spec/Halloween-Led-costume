# Halloween LED Costume

An Arduino Nano 33 BLE costume-lighting project with 29 addressable LEDs and
multiple animated effects. Press the button to switch between effects.

## Hardware

- Arduino Nano 33 BLE
- 29 HD107S addressable LEDs
- Push button connected to pin 12 and ground
- LED data on pin 11 and clock on pin 13

Use an appropriate power supply for the LEDs and connect its ground to the
Arduino ground.

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
