/// @file mos7_wago.h
/// @brief ������ � Wago ��� MINIOS7.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.
/// 
/// @par ������� ������:
/// @$Rev: 223 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-17 09:39:32#$.

#ifndef WAGO_MOS7_H
#define WAGO_MOS7_H

#include "wago.h"

//-----------------------------------------------------------------------------
/// @brief ������ � �������� �����/������ Wago ��� MINIOS7.
///
///
class wago_manager_mos7 : public wago_manager
    {
    public:
        wago_manager_mos7()
            {

            }

        virtual ~wago_manager_mos7()
            {

            }

        int read_inputs()
            {
            return 0;
            }

        int write_outputs()
            {
            return 0;
            }
    };
//-----------------------------------------------------------------------------
#endif // WAGO_MOS7_H


