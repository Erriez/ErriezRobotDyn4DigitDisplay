/*
 * MIT License
 *
 * Copyright (c) 2018 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \brief RobotDyn4DigitDisplay library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezRobotDyn4DigitDisplay
 *      Documentation:  https://erriez.github.io/ErriezRobotDyn4DigitDisplay
 */

#include <ErriezRobotDyn4DigitDisplay.h>

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

// Delay between display changes
#define DEMO_DELAY      800

// Create display object
RobotDyn4DigitDisplay display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// Declare variables
static int minute = 0;
static int second = 0;


void setup()
{
    // Initialize TM1637
    display.begin();

    // Clear display
    display.clear();

    // Set initial time
    display.setBrightness(0);
    display.time(12, 0);    // 1 2 : 0 0
    delay(DEMO_DELAY);

    // Change brightness
    for (uint8_t brightness = 1; brightness < 8; brightness++) {
        // Set brightness 0..7
        display.setBrightness(brightness);
        delay(DEMO_DELAY);
    }

    // Set brightness
    display.setBrightness(3);

    // Display individual digits: 1 2 3 4
    display.digit(0, 1);
    display.digit(1, 2);
    display.digit(2, 3);
    display.digit(3, 4);
    delay(DEMO_DELAY);

    // Display overflow
    display.overflow();
    delay(DEMO_DELAY);

    // Display negative temperature
    display.rawDigit(0, SEGMENTS_MINUS);
    display.digit(1, 1);
    display.rawDigit(2, SEGMENTS_DEGREE);
    display.rawDigit(3, SEGMENTS_CELSIUS);
    delay(DEMO_DELAY);

    // Display temperature with raw digits
    display.rawDigit(0, 0b01011011);
    display.rawDigit(1, 0b01111111);
    display.rawDigit(2, SEGMENTS_DEGREE);
    display.rawDigit(3, SEGMENTS_CELSIUS);
    delay(DEMO_DELAY);

    // Display error: E r r _
    display.rawDigit(0, 0b01111001);
    display.rawDigit(1, 0b01010000);
    display.rawDigit(2, 0b01010000);
    display.rawDigit(3, 0b00000000);

    // Display rect
    display.rawDigit(0, 0b00111001);
    display.rawDigit(1, 0b00001001);
    display.rawDigit(2, 0b00001001);
    display.rawDigit(3, 0b00001111);

    // Display positive and negative decimal numbers
    display.dec(0);         // _ _ _ 0
    delay(DEMO_DELAY);
    display.dec(1);         // _ _ _ 1
    delay(DEMO_DELAY);
    display.dec(34, 3);     // _ 0 3 4
    delay(DEMO_DELAY);
    display.dec(9999);      // 9 9 9 9
    delay(DEMO_DELAY);
    display.dec(-1);        // _ _ - 1
    delay(DEMO_DELAY);
    display.dec(-10);       // _ - 1 0
    delay(DEMO_DELAY);
    display.dec(-99);       // _ - 9 9
    delay(DEMO_DELAY);
    display.dec(-999);      // - 9 9 9
    delay(DEMO_DELAY);
    display.dec(10000);     // - - - -
    delay(DEMO_DELAY);

    // Display hexadecimal numbers
    display.hex(0);         // _ _ _ 0
    delay(DEMO_DELAY);
    display.hex(1, 2);      // _ _ 0 1
    delay(DEMO_DELAY);
    display.hex(0x34, 3);   // _ 0 3 4
    delay(DEMO_DELAY);
    display.hex(0x9999);    // 9 9 9 9
    delay(DEMO_DELAY);
    display.hex(-1);        // _ _ - 1
    delay(DEMO_DELAY);
    display.hex(0x1234);    // 1 2 3 4
    delay(DEMO_DELAY);
    display.hex(0xABCD);    // A B C D
    delay(DEMO_DELAY);
    display.hex(0xBEEF);    // B E E F
    delay(DEMO_DELAY);
    display.hex(0xFFFF);    // F F F F
    delay(DEMO_DELAY);
    display.hex(0x10000); // Overflow only displayed on > 32-bit systems
    delay(DEMO_DELAY);
}

void loop()
{
    // Display minutes:seconds with double dot on, padding hours off
    display.time(minute, second, true, false);
    delay(500);

    // Turn double dot off
    display.doubleDots(false);
    delay(500);

    // Increment time
    if (second >= 59) {
        second = 0;
        if (minute >= 59) {
            minute = 0;
        } else {
            minute++;
        }
    } else {
        second++;
    }
}
