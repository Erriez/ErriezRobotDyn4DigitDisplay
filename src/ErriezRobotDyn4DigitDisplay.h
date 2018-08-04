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
 * \file ErriezErriezRobotDyn4DigitDisplay.h
 * \brief RobotDyn4DigitDisplay library for Arduino
 * \details
 *      Source:         https://github.com/Erriez/ErriezRobotDyn4DigitDisplay
 *      Documentation:  https://erriez.github.io/ErriezRobotDyn4DigitDisplay
 */

#ifndef ERRIEZ_ROBOT_DYN_4_DIGIT_DISPLAY_H_
#define ERRIEZ_ROBOT_DYN_4_DIGIT_DISPLAY_H_

#include <Arduino.h>
#include <ErriezTM1637.h>

//! Number of display digits
#define ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS     4

//! Special characters
#define SEGMENTS_MINUS      0b01000000    //!< Minus sign
#define SEGMENTS_DEGREE     0b01100011    //!< Degree symbol
#define SEGMENTS_CELSIUS    0b00111001    //!< Celsius symbol

/*!
 * \brief RobotDyn4DigitDisplay class
 * \details
 *      This class
 */
class RobotDyn4DigitDisplay : public TM1637
{
public:
    // Constructor
    RobotDyn4DigitDisplay(uint8_t clkPin, uint8_t dioPin,
                          bool displayOn=true, uint8_t brightness=5);

    // Display functions
    void rawDigit(uint8_t digit, uint8_t value);
    void digit(uint8_t digit, uint8_t value);
    void doubleDots(bool on);
    void time(uint8_t hour, uint8_t minute, bool doubleDotsOn=true, bool padHours=true);
    void dec(int value, uint8_t pad=1);
    void hex(unsigned int value, uint8_t pad=4);
    void overflow();

private:
    //! Display buffer which contains segment data
    uint8_t displayBuffer[ROBOT_DYN_4DIGIT_DISPLAY_NUM_DIGITS];
};

#endif // ERRIEZ_ROBOT_DYN_4_DIGIT_DISPLAY_H_
