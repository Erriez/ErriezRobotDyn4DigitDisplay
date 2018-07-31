# RobotDyn 4-digit LED display with TM1637 library for Arduino.

[![Build Status](https://travis-ci.org/Erriez/ErriezErriezRobotDyn4DigitDisplay.svg?branch=master)](https://travis-ci.org/Erriez/ErriezErriezRobotDyn4DigitDisplay)

This is a RobotDyn 4-digit 7-segment LED display library for Arduino. The PCB contains a two wire [TM1637 LED / button](https://github.com/Erriez/ErriezTM1637) controller.

![ErriezRobotDyn4DigitDisplay](https://raw.githubusercontent.com/Erriez/ErriezErriezRobotDyn4DigitDisplay/master/extras/ErriezRobotDyn4DigitDisplay.png)

**Note**: This library uses the double-dot to display a time. The LED dots per segment are not wired and cannot be controlled.

## Library features

- Set brightness (0..7)
- Set digit (0..3)
- Control all individual segments per digit
- Control double dots (on/off)
- Display time (hours:minutes)
- Display decimal value (-999..9999) with optional padding
- Display hexadecimal value (0...0xFFFF) with optional padding

## Hardware

**Connection display with Arduino**

| Display | Arduino UNO / Nano / Pro Mini / Leonardo / Mega2560 / ESP8266 / Lolin32 |
| :-----: | :----------------------------------------------------------- |
|   GND   | GND                                                          |
|   VCC   | 5V (or 3.3V)                                                 |
|   CLK   | Any DIGITAL pin                                              |
|   DIO   | Any DIGITAL pin                                              |

Other MCU's may work, but are not tested.


## Examples

Examples | Erriez RobotDyn 4-digit display | [7SegementDisplayDemo](https://github.com/Erriez/ErriezRobotDyn4DigitDisplay/blob/master/examples/7SegementDisplayDemo/7SegementDisplayDemo.ino)

## Usage

**Initialization**

```c++
#include <RobotDyn4DigitDisplay.h>

// Connect display pins to the Arduino DIGITAL pins
#if defined(ARDUINO_ARCH_AVR)
#define TM1637_CLK_PIN      2
#define TM1637_DIO_PIN      3
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI) || defined(ESP8266_WEMOS_D1MINI) || defined(ARDUINO_ESP8266_NODEMCU)
#define TM1637_CLK_PIN      D2
#define TM1637_DIO_PIN      D3
#elif defined(ARDUINO_LOLIN32)
#define TM1637_CLK_PIN      0
#define TM1637_DIO_PIN      4
#else
#error "May work, but not tested on this target"
#endif

// Create display object
RobotDyn4DigitDisplay display(TM1637_CLK_PIN, TM1637_DIO_PIN);

void setup()
{
    // Initialize TM1637
    display.begin();
}
```

**Clear display**

```c++
// Clear display
display.clear();	// _ _ _ _
```

**Set brightness**

```c++
// Set brightness
display.setBrightness(0); // Minimum
display.setBrightness(7); // Maximum
```

**Display time**

```c++
// Display time
display.time(11, 59);	// 1 1 : 5 9
```

**Control time double dot**

```c++
display.doubleDots(true); 	// Turn double dot on
display.doubleDots(false); 	// Turn double dot off
```

**Display decimal value**

```c++
// Display decimal values
display.dec(-999);	// - 9 9 9
display.dec(-1);	// _ _ - 1
display.dec(0);		// _ _ _ 0
display.dec(1);		// _ _ _ 1
display.dec(123);	// _ 1 2 3
display.dec(9999);	// 9 9 9 9
display.dec(10000);	// - - - -

// Display decimal values with padding
display.dec(1); 	// _ _ _ 1  (Default no padding)
display.dec(1, 2);	// _ _ 0 1  (2 digits padding)
display.dec(1, 3);	// _ 0 0 1  (3 digits padding)
display.dec(1, 4);	// 0 0 0 1  (4 digits padding)

display.dec(34, 3);	// _ 0 3 4  (2 digits padding)
```

**Display hexadecimal value**

```c++
// Display hexadecimal values
display.dec(0x0000);	// 0 0 0 0
display.dec(0x1234);	// 1 2 3 4
display.dec(0xABCD);	// A B C D
display.dec(0xBEEF);	// B E E F

// Display hexadecimal values with padding
display.hex(0x0001); 	// _ _ _ 1  (Default no padding)
display.hex(0x0001, 2);	// _ _ 0 1  (2 digits padding)
display.hex(0x0001, 3);	// _ 0 0 1  (3 digits padding)
display.hex(0x0001, 4);	// 0 0 0 1  (4 digits padding)

display.hex(0x0034, 3);	// _ 0 3 4  (2 digits padding)
```

**Control individual digits**

```c++
// Display individual digits: 1 2 3 4
display.digit(0, 1);
display.digit(1, 2);
display.digit(2, 3);
display.digit(3, 4);
```

**Special characters**

```c++
Control individual LED-segments (bit numbers):
   - 0 -
   |   |
   5   1
   |   |
   - 6 -
   |   |
   4   2
   |   |
   - 3 -  .7

// Display error: E r r _
display.rawDigit(0, 0b01111001);
display.rawDigit(1, 0b01010000);
display.rawDigit(2, 0b01010000);
display.rawDigit(3, 0b00000000);

// Display H character: _ _ _ H
display.rawDigit(3, 0b01110110);

// Display negative temperature: - 1 ` C
display.rawDigit(0, SEGMENTS_MINUS);
display.digit(1, 1);
display.rawDigit(2, SEGMENTS_DEGREE);
display.rawDigit(3, SEGMENTS_CELSIUS);

// Display rect
display.rawDigit(0, 0b00111001);
display.rawDigit(1, 0b00001001);
display.rawDigit(2, 0b00001001);
display.rawDigit(3, 0b00001111);
```

## Documentation

* [Doxygen online HTML](https://erriez.github.io/ErriezErriezRobotDyn4DigitDisplay)


## Library dependencies

* [Erriez TM1637](https://github.com/Erriez/ErriezTM1637) library


## Library installation

Please refer to the [Wiki](https://github.com/Erriez/ErriezArduinoLibrariesAndSketches/wiki) page.


## Other Arduino Libraries and Sketches from Erriez

* [Erriez Libraries and Sketches](https://github.com/Erriez/ErriezArduinoLibrariesAndSketches)
