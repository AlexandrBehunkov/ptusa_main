/// @file wago_w750.h
/// @brief ������ � Wago ��� PAC WAGO 750-860.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.
/// 
/// @par ������� ������:
/// @$Rev: 79 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-05-14 09:54:41#$.

#ifndef WAGO_750_H
#define WAGO_750_H

#include "wago.h"

#include "kbusapi.h"
//-----------------------------------------------------------------------------
/// @brief ������ � �������� �����/������ Wago ��� PAC WAGO 750-860.
///
///
class wago_manager_w750 : public wago_manager
    {
    public:
        wago_manager_w750();

        virtual ~wago_manager_w750();

        int read_inputs();
        int write_outputs();
    };
//-----------------------------------------------------------------------------
#endif // WAGO_750_H
