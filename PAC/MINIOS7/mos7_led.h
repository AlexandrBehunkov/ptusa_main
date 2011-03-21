/// @file mos7_led.h
/// @brief ������ �� ������������ ����������� � �� MINIOS7.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
/// 
/// @par ������� ������:
/// @$Rev: 220 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-15 16:58:56#$.

#ifndef LED_MINIOS7_H
#define LED_MINIOS7_H

#include "led.h"

//-----------------------------------------------------------------------------
/// @brief ������ � �������� ������������.
class led_mos7 : public led
    {
    public:
        void on( LEDS_NAMES led, COLOUR led_color )
            {
            }

        void off( LEDS_NAMES led )
            {
            }

        void blink( LEDS_NAMES led, COLOUR led_color )
            {
            }
    };
//-----------------------------------------------------------------------------
#endif // LED_MINIOS7_H
