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
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef WAGO_750_H
#define WAGO_750_H

#include "l_wago.h"

#include "kbusapi.h"
//-----------------------------------------------------------------------------
/// @brief ������ � �������� �����/������ Wago ��� PAC WAGO 750-860.
///
///
class wago_manager_w750 : public wago_manager_linux
    {
     public:
        wago_manager_w750();

        virtual ~wago_manager_w750();

        int read_inputs();
        int write_outputs();
    };
//-----------------------------------------------------------------------------
#endif // WAGO_750_H
