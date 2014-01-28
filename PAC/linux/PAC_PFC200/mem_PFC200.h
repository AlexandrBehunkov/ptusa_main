/// @file mem_PFC200.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� PAC
/// �� ������ � �������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @par ������� ������:
/// @$Rev: 220 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-15 16:58:56#$.

#ifndef MEM_PFC200_H
#define MEM_PFC200_H

#include "base_mem.h"
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ��� (Static Memory).
///
/// ����� ����������� �� ���������� ������ ������/������ - 1 �������.
class eeprom_PFC200: public NV_memory
    {
    friend class NV_memory_manager;

    private:
        eeprom_PFC200( int total_size, int available_start_pos,
            int available_end_pos );
        
        virtual ~eeprom_PFC200();
        
        /// @brief ����� ���������� @ref i_memory.
        int read( void *buff, u_int count, u_int start_pos );

        /// @brief ����� ���������� @ref i_memory.
        int write( void *buff, u_int count, u_int start_pos );
        
        char * NV_ram_data; 	// Pointer to begin of NVRAM
    };
//-----------------------------------------------------------------------------
#endif // MEM_PFC200_H