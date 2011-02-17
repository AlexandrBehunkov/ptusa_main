/// @file sys_7186.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� 
/// PAC 7186EXD.
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

typedef int             int_2;
typedef unsigned int    u_int_2;

typedef unsigned long   u_int_4;
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ��� (Electrically Erasable 
/// Programmable Read-Only Memory).
///
/// ����� ����������� �� ���������� ������ ������/������ - 1 �������.
class EEPROM_7186: public NV_memory 
    {
    public:
        EEPROM_7186();

        /// @brief ����� ���������� @ref i_memory.
        int read( void *buff, u_int count, u_int start_pos );

        /// @brief ����� ���������� @ref i_memory.
        int write( void *buff, u_int count, u_int start_pos );
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ��� (non-volatile random access 
/// memory).
///
/// ������ ������ �������� �� ���������, �� ����� ����������� ��  
/// ���������� ������ ������/������.
class NVRAM_7186: public NV_memory
    {
    public:
        NVRAM_7186();

        /// @brief ����� ���������� @ref i_memory.
        int read( void *buff, u_int count, u_int start_pos );

        /// @brief ����� ���������� @ref i_memory.
        int write( void *buff, u_int count, u_int start_pos );
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ���. ������������ ���������� �� 
/// ����������� ������������� ������.
class NV_memory_manager_7186: public NV_memory_manager
    {    
    public:
        NV_memory_manager_7186();
    };
//-----------------------------------------------------------------------------
#endif SYS_7186_H