/// @file sys_linux.h
/// @brief �������� �������� �������, ������� ��������� ���������� �����
/// PAC ��� �� linux.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef SYS_LINUX
#define SYS_LINUX

#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#include "sys.h"

typedef short int            int_2;
typedef int                  int_4;
typedef unsigned short int u_int_2;

typedef unsigned int       u_int_4;
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ��� (Static Memory).
///
/// ����� ����������� �� ���������� ������ ������/������ - 1 �������.
class SRAM: public NV_memory
    {
    friend class NV_memory_manager_W750;
    friend class NV_memory_manager_PC;

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
#endif // SYS_LINUX
