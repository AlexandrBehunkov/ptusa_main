/// @file sys_w750.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� 
/// PAC WAGO 750.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef SYS_7186_H
#define SYS_7186_H

#include "sys.h"

typedef short int          int_2;
typedef int                int_4;
typedef unsigned short int u_int_2;

typedef unsigned int       u_int_4;
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ��� (Static Memory).
///
/// ����� ����������� �� ���������� ������ ������/������ - 1 �������.
class SRAM: public NV_memory 
    {
    friend class NV_memory_manager_W750;

    private:

        static int file;

        SRAM( u_int total_size, u_int available_start_pos, 
            u_int available_end_pos );
        virtual ~SRAM();

        /// @brief ����� ���������� @ref i_memory.
        int read( void *buff, u_int count, u_int start_pos );

        /// @brief ����� ���������� @ref i_memory.
        int write( void *buff, u_int count, u_int start_pos );
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ���. ������������ ���������� �� 
/// ����������� ������������� ������.
class NV_memory_manager_W750: public NV_memory_manager
    {    
    public:
        NV_memory_manager_W750();
    };
//-----------------------------------------------------------------------------
#endif // SYS_7186_H
