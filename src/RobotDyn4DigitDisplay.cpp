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
 * \file RobotDyn4DigitDisplay.cpp
 * \brief RobotDyn4DigitDisplay library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezRobotDyn4DigitDisplay
 *      Documentation:  https://erriez.github.io/ErriezRobotDyn4DigitDisplay
 */

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include "RobotDyn4DigitDisplay.h"

/* 7-segment bits:
   - 0 -
   |   |
   5   1
   |   |
   - 6 -
   |   |
   4   2
   |   |
   - 3 -  .7
*/

//! 7-segment display digit defines
static const PROGMEM uint8_t SEGMENT_DATA[16] = {
    0b00111111, /* 0 */
    0b00000110, /* 1 */
    0b01011011, /* 2 */
    0b01001111, /* 3 */
    0b01100110, /* 4 */
    0b01101101, /* 5 */
    0b01111101, /* 6 */
    0b00000111, /* 7 */
    0b01111111, /* 8 */
    0b01101111, /* 9 */
    0b01110111, /* A */
    0b01111100, /* B */
    0b00111001, /* C */
    0b01011110, /* D */
    0b01111001, /* E */
    0b01110001, /* F */
};

/*!
 * \brief Constructor RobotDyn 4-digit LED display
 * \param clkPin
 *      Clock pins.
 * \param dioPin
 *      Bi-directional data pin.
 * \param displayOn
 *      Optional: Turn display on. Default: true
 * \param brightness
 *      Optional: Set brightness 0..7 Default: 5.
 */
RobotDyn4DigitDisplay::RobotDyn4DigitDisplay(uint8_t clkPin, uint8_t dioPin, bool displayOn,
                                             uint8_t brightness) :
        TM1637(clkPin, dioPin, displayOn, brightness)
{

}

/*!
 * \brief Display raw digit
 * \param digit
 *      Digit number 0 (left digit) .. 3 (right digit)
 * \param value
 *      LED segments
 */
void RobotDyn4DigitDisplay::rawDigit(uint8_t digit, uint8_t value)
{
    if (digit < ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS) {
        displayBuffer[digit] = value;
        writeData(digit, &displayBuffer[digit], 1);
    }
}

/*!
 * \brief Display a single digit.
 * \param digit
 *      Digit number 0 (left digit) ... 3 (right digit)
 * \param value
 *      Digit value 0..9 or 0x00..0x0F.
 */
void RobotDyn4DigitDisplay::digit(uint8_t digit, uint8_t value)
{
    if (value < sizeof(SEGMENT_DATA)) {
        rawDigit(digit, pgm_read_byte(&SEGMENT_DATA[value]));
    }
}

/*!
 * \brief Display double time dots
 * \param on
 *      true: Turn double time dots on.\n
 *      false: Turn double time dots off.
 */
void RobotDyn4DigitDisplay::doubleDots(bool on)
{
    if (on) {
        displayBuffer[1] |= 0x80;
    } else {
        displayBuffer[1] &= 0x7f;
    }
    writeData(0x01, &displayBuffer[1], 1);
}

/*!
 * \brief Display time
 * \param hour
 *      0..59: Hours
 * \param minute
 *      0..59: Minutes
 * \param doubleDotsOn
 *      true: Display double time dots. (Default)\n
 *      false: Turn double time dots off.
 * \param padHours
 *      true: Display first digit as 0 when hours < 10.
 *      false: Turn first digit off when hours < 10.
 */
void RobotDyn4DigitDisplay::time(uint8_t hour, uint8_t minute, bool doubleDotsOn,
                                        bool padHours)
{
    if (padHours || (hour >= 10)) {
        displayBuffer[0] = pgm_read_byte(&SEGMENT_DATA[hour / 10]);
    } else {
        displayBuffer[0] = 0;
    }
    displayBuffer[1] = pgm_read_byte(&SEGMENT_DATA[hour % 10]) | (doubleDotsOn ? 0x80 : 0x00);
    displayBuffer[2] = pgm_read_byte(&SEGMENT_DATA[minute / 10]);
    displayBuffer[3] = pgm_read_byte(&SEGMENT_DATA[minute % 10]);
    writeData(0x00, displayBuffer, sizeof(displayBuffer));
}

/*!
 * \brief Display decimal value.
 * \param value
 *      0000..9999: Decimal value.
 * \param pad
 *      0..4: Optional: Number of digits to pad with a zero. Default: 1.
 */
void RobotDyn4DigitDisplay::dec(int value, uint8_t pad)
{
    uint8_t digit;
    bool negative = false;

    // Check if value fits on display
    if ((value > 9999) || (value < -999)) {
        overflow();
        return;
    }

    if (value < 0) {
        // Make value positive
        value *= -1;

        // Set negative flag
        negative = true;
    }

    // Clear buffer
    memset(displayBuffer, 0x00, ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS);

    // Value 0..9999 with padding
    for (uint8_t i = 0; i < ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS; i++) {
        digit = (uint8_t)(ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS - 1 - i);
        if (value == 0) {
            if (pad > i) {
                displayBuffer[digit] = pgm_read_byte(&SEGMENT_DATA[0]);
            } else {
                displayBuffer[digit] = 0;
            }
        } else {
            displayBuffer[digit] = pgm_read_byte(&SEGMENT_DATA[value % 10]);
        }

        value /= 10;

        if ((value == 0) && negative) {
            displayBuffer[--digit] = SEGMENTS_MINUS;
            break;
        }
    }

    // Write display segment data
    writeData(0x00, displayBuffer, sizeof(displayBuffer));
}

/*!
 * \brief Display hexadecimal value with optional padding
 * \param value
 *      0x0000..0xFFFF: Hexadecimal value
 * \param pad
 *      0..4: Optional: Number of digits to pad with a zero. Default: 4.
 */
void RobotDyn4DigitDisplay::hex(unsigned int value, uint8_t pad)
{
    uint8_t digit;

    // Check if value fits on display
    if (value > 0xFFFF) {
        // Reached only when unsigned int > 16-bit
        overflow();
        return;
    }

    for (uint8_t i = 0; i < ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS; i++) {
        digit = (uint8_t)(ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS - 1 - i);
        if (value == 0) {
            if (pad > i) {
                displayBuffer[digit] = pgm_read_byte(&SEGMENT_DATA[0]);
            } else {
                displayBuffer[digit] = 0;
            }
        } else {
            displayBuffer[digit] = pgm_read_byte(&SEGMENT_DATA[value % 16]);
        }

        value /= 16;
    }

    // Write display segment data
    writeData(0x00, displayBuffer, sizeof(displayBuffer));
}

/*!
 * \brief Display overflow with four minus digits
 */
void RobotDyn4DigitDisplay::overflow()
{
    // Display minus char on value area
    for (uint8_t i = 0; i < ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS; i++) {
        displayBuffer[i] = SEGMENTS_MINUS;
    }

    // Write display segment data
    writeData(0x00, displayBuffer, sizeof(displayBuffer));
}
