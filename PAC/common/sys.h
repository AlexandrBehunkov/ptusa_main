/// @file sys.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� PAC.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @c WIN32    - ���������� ��� OS Windows.@n
/// @c PAC_W750 - ���������� ��� PAC Wago 750.@n
/// @c I7186_E  - ���������� ��� PAC I7186_E. ������ ��������� ����������
/// ����������� ��� ��������� ���������� ������ � ��������, ���� ���������
/// ��������� �� ������� � ���������� ������.
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef SYS_H
#define SYS_H

typedef unsigned char           u_char;
typedef unsigned int            u_int;
typedef unsigned long int       u_long;

#include <string.h>
#include <stdio.h>

#include "util.h"
//-----------------------------------------------------------------------------
#ifdef DEBUG
/// @brief �������� ������� �������.
///
/// @return 0 - ��� ������� ������.
/// @return 1 - ������ �������.
int  kbhit();

/// @brief ���������� �������� ������� � �������.
///
/// @return - ������ ������� �������.
int  get_char();

/// @brief ����� ����� � ������� � �������� ����.
///
/// @param � - ��������� �����.
void print_binary( u_int c );
#endif // DEBUG
//-----------------------------------------------------------------------------
#if defined LINUX || defined UCLINUX
#include <time.h>

#define print_time printf( "%02lu:%02lu:%02lu ", ( time( 0 ) / 3600 ) % 24, \
    ( time( 0 ) / 60 ) % 60, time( 0 ) % 60 ); printf

#define Print printf

#endif // defined LINUX || defined UCLINUX
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������� � ��������.
///
/// @return ������� ����� (������� � 01.01.1970, � 2038 ���������� ������������).
u_long get_sec();
//-----------------------------------------------------------------------------
/// @brief ��������� ������� � �������������.
///
/// ��� ��� �� 4 ��� ���������� ������������ � ������ ������������ � 0, �� ���
/// ���������� �������� ������� ������������ @ref get_delta_millisec.
///
/// @return ����� � ������� ������� ��������� � ��������������.
u_long get_millisec();
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������� � �������������.
///
/// @param time1     - ��������� �����.
/// @return �������� ������� � �������������.
u_long get_delta_millisec( u_long time1 );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� � ������.
class i_memory
    {
    public:
        /// @brief ������ ������� ����.
        ///        
        /// @param buf       - ����� ������, ���� ����� ����������� ������.
        /// @param count     - ���������� ����������� ����.
        /// @param start_pos - ��������� �����.
        ///
        /// @return <  0 - ������.
        /// @return >= 0 - ���������� ��������� ����.
        virtual int read( void *buf, u_int count, u_int start_pos ) = 0;

        /// @brief ������ ������� ����.
        ///        
        /// @param buf       - ����� ������, ���� ����� ������������ ������.
        /// @param count     - ���������� ����������� ����.
        /// @param start_pos - ��������� �����.
        ///
        /// @return <  0 - ������.
        /// @return >= 0 - ���������� ���������� ����.
        virtual int write( void *buf, u_int count, u_int start_pos ) = 0;

        /// @brief ��������� ������� ������ � ������.
        ///        
        /// @return - ������ ������ � ������.
        virtual u_int get_size() const = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ������ �������� � ����������������� ���. ������������ ���������� �� 
/// ���������� ���������� ������.
class NV_memory : public i_memory
    {  

    public:
        /// @param total_size           - ����� ������ ������.
        /// @param available_start_pos  - ��������� ��������� �����.
        /// @param available_end_pos    - �������� ��������� �����.
        NV_memory( u_int total_size, u_int available_start_pos, 
            u_int available_end_pos );

        /// @brief ����� ���������� @ref i_memory.
        u_int get_size() const
            {
            return total_size;
            }

        /// @brief ��������� ���������� ���������� ��� ������ ������ ������.
        ///        
        /// @return - ��������� ��������� ��� ������ ����� ������.
        u_int get_available_start_pos() const
            {
            return available_start_pos;
            }

        /// @brief ��������� ��������� ���������� ��� ������ ������ ������.
        ///        
        /// @return - �������� ��������� ��� ������ ����� ������.
        u_int get_available_end_pos() const
            {
            return available_end_pos;
            }

    private:

        u_int total_size;           ///< ����� ������ ������.

        /// @brief ��������� ��������� �����.
        /// @details ��� �������� ����������������� ��������� �������.
        u_int available_start_pos;      

        /// @brief �������� ��������� �����.
        /// @details ��� �������� ����������������� ��������� �������.
        u_int available_end_pos;        
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������ ������.
class memory_range: public i_memory
    {
    friend class NV_memory_manager;

    public:
        /// @brief ����� ���������� @ref i_memory.
        u_int get_size() const
            {
            return size;
            }

        /// @brief ����� ���������� @ref i_memory.
        int read( void *buf, u_int count, u_int start_pos );

        /// @brief ����� ���������� @ref i_memory.
        int write( void *buf, u_int count, u_int start_pos );

    private:
        i_memory    *memory;    ///< ��������� �� ������ ������.
        u_int       start_pos;  ///< ��������� �����.
        u_int       size;       ///< ������ ����� ������ � ������.

        /// @brief �������� �����������.
        ///
        /// �������� �������� �������������� ����� ����� get_memory_block
        /// ������ @ref NV_memory_manager.
        memory_range( i_memory *memory, u_int start_pos, u_int size );

        /// @brief �������� �� ������������ ����������.
        ///
        /// @return - ��������� ��������
        ///    0 - ��.
        ///    1 - ������.
        int check_params( u_int count, u_int start_pos );
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������������� ���. ������������ ���������� �� 
/// ����������� ������������� ������.
class NV_memory_manager
    {
    public:        
        enum MEMORY_TYPE ///< ���� ����������������� ������.
            {
            MT_NVRAM,
            MT_EEPROM
            };

        /// @brief ��������� ������� ��� ������ � ������ ������.
        ///        
        /// @param m_type - ��� ������, ������ ����� ��������� ������.
        /// @param count  - ������ � ������.
        ///
        /// @return =  0 - ������.
        /// @return >  0 - ��������� �� ������ ����������� ����� ������.
        virtual memory_range* get_memory_block( MEMORY_TYPE m_type,
            u_int count );

        /// @brief ��������� ������������� ���������� ������ ��� ������ � 
        /// �����������.
        ///
        /// @return - ��������� �� ������������ ������ ������ @ref
        /// NV_memory_manager.
        static NV_memory_manager* get_instance();

        /// @brief ��������� ������������� ���������� ������ ��� ������ � 
        /// �����������.
        ///
        /// @param new_instance - ����� ��������� �� ������������ ������ 
        /// ������ @ref NV_memory_manager.
        static void set_instance( NV_memory_manager* new_instance );

        virtual ~NV_memory_manager();

    protected:
        /// ����������� ��������� ������ ��� ������ �������.
        static auto_smart_ptr < NV_memory_manager > instance;

        NV_memory_manager();

        /// @brief ������ � ����������������� ��� (non-volatile random access 
        /// memory).
        ///
        /// ������ ������ �������� �� ���������, �� ����� ����������� ��  
        /// ���������� ������ ������/������.
        NV_memory *PAC_NVRAM;

        /// @brief ������ � ����������������� ��� (Electrically Erasable 
        /// Programmable Read-Only Memory).
        ///
        /// ����� ����������� �� ���������� ������ ������/������ - 1 �������.
        NV_memory *PAC_EEPROM;

        u_int last_NVRAM_pos;  ///< ������ �������� ���������� �������� NVRAM.
        u_int last_EEPROM_pos; ///< ������ �������� ���������� �������� EEPROM.
    };
//-----------------------------------------------------------------------------
/// @brief ������ � �������� �������� �������. ������������ ���������� �� 
/// ���������� ���������� �������.
class file
    {
    public:
        /// @brief �������� �����.
        ///
        /// @param file_name - ��� �����. 
        ///
        /// @return - ��������� 
        ///    0   - ��.
        ///    0 < - ������.
        virtual int fopen( const char *file_name ) = 0;

        /// @brief ������ �� ����� ����� ������.
        ///
        /// @param buffer - �����, ���� ��������� ���������. 
        /// @param count - ���������� ����������� ����. 
        ///
        /// @return - ���������� ��������� ����. 
        virtual int fread( void *buffer, int count ) = 0;

        /// @brief ������ �� ����� ������.
        ///
        /// @return - ��������� �� ������ ������. 
        virtual char* fget_line() = 0;

        /// @brief ������ �� ����� ������ ��� ��������� ������� ��������� � 
        /// �����.
        ///
        /// @return - ��������� �� ������ ������. 
        virtual char* pfget_line() = 0;

        /// @brief �������� �����.
        ///
        /// @return - ��������� 
        ///    0   - ��.
        ///    0 < - ������.
        virtual void fclose() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ������ � �������� ������������. ������������ ���������� ��
/// ���������� ���������� �������.
class led
    {
    public:
        enum COLOUR
            {
            C_GREEN = 1,
            C_YELLOW,
            C_RED
            };

        enum LEDS_NAMES
            {
            L_ALL = 1,
            L_STATUS,
            L_SERVICE,
            L_USER
            };

        /// @brief �������� ������� ���������.
        ///
        /// @param led       - ����.
        /// @param led_color - ����.
        virtual void on( LEDS_NAMES led, COLOUR led_color ) = 0;

        /// @brief ��������� ������� ���������.
        virtual void off( LEDS_NAMES led ) = 0;

        /// @brief �������� ������� �������� ����������.
        ///
        /// @param led       - ����.
        /// @param led_color - ����.
        virtual void blink( LEDS_NAMES led, COLOUR led_color ) = 0;
    };
//-----------------------------------------------------------------------------
#endif // SYS_H
