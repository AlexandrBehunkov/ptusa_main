/// @file PAC_err.h
/// @brief �������� �������� �������, ������� ������������ ��� ����������� 
/// ������ � ������������ �������� PAC.
/// 
/// ����� @ref PAC_critical_errors_manager, �������� ��� ����������� ������ � 
/// �������� ������� ���������, �������� ��� ����������� ����������. ��� ����� 
/// ���� ������ ������ ��������� @ref PAC_critical_errors_manager::critical_error. 
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c PAC_ERRORS_H - ��������� ������� ����� � ���������� ������ ���� ���.@n
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef PAC_ERRORS_H
#define PAC_ERRORS_H

#include <string.h>
#include <vector>

#include "errors.h"
#include "smart_ptr.h"
#include "dtime.h"
#include "led.h"

//-----------------------------------------------------------------------------


//0, 2, n - ��� ����� � ������� ����� n.
//0, 3, n - ��� ����� � Modbus ����������� ����� n.
//0, 4, 4 - ��� ����� � �������������� PAC.
//0, 5, 5 - ��� ����� � ��������.
//
//1, 1 - ������ ������ � COM-������ WAGO. 
//      1 - CRC error
//
//13 - ������ ������� ������:
//  1, n  - ������ ��������� ������ � ������� �������� ����� n. 
//-----------------------------------------------------------------------------
class PAC_critical_errors_manager
    {
    public:
        enum GE_CONST
            {
            GE_ERROR_SIZE = 3,      ///< ������ ����� ������, ����.
            };

        PAC_critical_errors_manager();        

        void show_errors();
        void set_global_error( ALARM_CLASS eclass, ALARM_SUBCLASS p1, 
            unsigned long param );
        void reset_global_error( ALARM_CLASS eclass, ALARM_SUBCLASS p1, 
            unsigned long param );

        int save_as_Lua_str( char *str, u_int_2 &id );

        static PAC_critical_errors_manager * get_instance();

        u_int get_id() const
            {
            return errors_id;
            }

    private:
        static auto_smart_ptr < PAC_critical_errors_manager > instance;

        struct critical_error
            {
            int             err_class;     ///< ����� ������.
            unsigned int    err_sub_class; ///< �������� ������.
            unsigned int    param;         ///< �������� ������.

            critical_error( int err_class = 0, u_int err_sub_class = 0,
                u_int param = 0 );
            };

        std::vector< critical_error >  errors;

        u_int_2 errors_id;
        int     global_ok;
    };
//-----------------------------------------------------------------------------
#endif // PAC_ERRORS_H
