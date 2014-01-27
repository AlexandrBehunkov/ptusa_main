/// @file wago_w750.h
/// @brief ������ � Wago ��� PAC WAGO 750-860.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG      - ���������� c ������� ���������� ���������� � �������.
/// @c DEBUG_KBUS - ���������� c ������� ���������� ���������� ������ KBUS �
/// �������.
///
/// @par ������� ������:
/// @$Rev: 588 $.\n
/// @$Author: id $.\n
/// @$Date:: 2013-01-21 08:47:30#$.

#ifndef WAGO_750_H
#define WAGO_750_H

#include "l_wago.h"

extern "C"
    {
    #include "kbusapi.h"
    }
//-----------------------------------------------------------------------------
/// @brief ������ � �������� �����/������ Wago ��� PAC WAGO 750-860.
///
///
class wago_manager_PFC200 : public wago_manager_linux
    {
     public:
        wago_manager_PFC200()
            {
            
            }

        virtual ~wago_manager_PFC200()
            {
            
            }

        int read_inputs()
            {
            return wago_manager_linux::read_inputs();
            }
        
        int write_outputs()
            {
            return wago_manager_linux::write_outputs();
            }
    };
//-----------------------------------------------------------------------------
#endif // WAGO_750_H
