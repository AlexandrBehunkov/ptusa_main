/// @file sys_w750.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� 
/// PAC WAGO 750.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// 
/// @par ������� ������:
/// @$Rev: 65 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-05-06 12:38:39#$.

#ifndef SYS_PC_H
#define SYS_PC_H

#include "sys_win.h"
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ���. 
class NV_memory_manager_PC: public NV_memory_manager
    {    
    public:
        NV_memory_manager_PC();       
    };
//-----------------------------------------------------------------------------
/// @brief ������ � �������� ������������.
class led_PC : public led
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
#endif // SYS_PC_H
