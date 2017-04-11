/// @file win\PAC_PC\wago_PC.h
/// @brief ������ � Wago ��� PC.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef WAGO_PC_H
#define WAGO_PC_H

#include "wago.h"

//-----------------------------------------------------------------------------
/// @brief ������ � �������� �����/������ Wago ��� PC.
///
///
class wago_manager_PC : public wago_manager
    {
    public:
        wago_manager_PC();

        virtual ~wago_manager_PC();

        int read_inputs();
        int write_outputs();
    };
//-----------------------------------------------------------------------------
#endif // WAGO_PC_H


