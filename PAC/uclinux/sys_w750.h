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
#include <fcntl.h>
#include <unistd.h>

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
class file_w750 : public file
    {
    public:
        file_w750() : f( 0 )
            {
            }
        
        int fopen( const char* file_name )
            {
            f = open( file_name, O_RDONLY );
            return f;
            }

        int fread( void *buffer, int count )
            {
            int res = 0;
            if ( f > 0 )
                {
                res = read( f, buffer, count );
                }

            return res;
            }

        char* fget_line()
            {
            buf[ 0 ] = 0;

            char *tmp_buff = buf;
            int pos = 1;
            read( f, tmp_buff, 1 );
            while ( tmp_buff[ 0 ] != '\n' )
                {
                tmp_buff++;
                int res = 0;

                res = read( f, tmp_buff, 1 );
           

                if ( res != 1 )
                    {
#ifdef DEBUG
                    Print( "Error reading file - can\'t read more!\n" );
#endif // DEBUG
                    break;
                    }

                pos++;
                if ( pos >= C_MAX_BUFFER_SIZE )
                    {
#ifdef DEBUG
                    Print( "Error reading file - line is too long!\n" );
#endif // DEBUG
                    break;
                    }
                }

            buf[ pos ] = 0;
            return buf;
            }

        char* pfget_line()
            {
            fget_line();
            int str_len = strlen( buf );
            lseek( f, -str_len, SEEK_CUR );
            return buf;
            }

        void fclose()
            {
            if ( f > 0 )
                {
                close( f );
                f = 0;
                }
            }
            

    private:
        enum CONSTANTS
            {
            C_MAX_BUFFER_SIZE = 200,
            };

        int f;
        char buf[ C_MAX_BUFFER_SIZE ];
    };
//-----------------------------------------------------------------------------
#endif // SYS_7186_H
