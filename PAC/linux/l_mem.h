/// @file l_mem.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� PAC
/// �� ������ � �������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev: 220 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-15 16:58:56#$.

#ifndef L_MEM_H
#define L_MEM_H

#include "base_mem.h"
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ��� (Static Memory).
///
/// ����� ����������� �� ���������� ������ ������/������ - 1 �������.
class SRAM: public NV_memory
    {
    friend class NV_memory_manager;

    private:
        enum CONSTANTS
            {
            C_MAX_FILE_NAME = 200
            };

        char file_name[ 200 ];

        static int file;

        SRAM( const char *file_name, u_int total_size, u_int available_start_pos,
            u_int available_end_pos );
        virtual ~SRAM();

        /// @brief ����� ���������� @ref i_memory.
        int read( void *buff, u_int count, u_int start_pos );

        /// @brief ����� ���������� @ref i_memory.
        int write( void *buff, u_int count, u_int start_pos );
    };
//-----------------------------------------------------------------------------
class data_file : public file
    {
    public:
        data_file();

        int file_open( const char* file_name );

        int file_read( void *buffer, int count );

        char* fget_line();

        char* pfget_line();

        void file_close();

    private:
        enum CONSTANTS
            {
            C_MAX_BUFFER_SIZE = 200
            };

        int f;
        char buf[ C_MAX_BUFFER_SIZE ];
    };
//-----------------------------------------------------------------------------
#endif // L_MEM_H
