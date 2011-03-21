/// @file mos7_mem.h
/// @brief �������� �������� �������, ������� ��������� ���������� �����
/// ������ � ������� � MINIOS7.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev: 223 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-17 09:39:32#$.

#ifndef MINIOS7_H
#define MINIOS7_H

#include "base_mem.h"
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
#endif MINIOS7_H