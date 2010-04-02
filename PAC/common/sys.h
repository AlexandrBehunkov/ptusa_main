/// @file sys.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� PAC.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @c WIN32   - ���������� ��� OS Windows.@n
/// @c W750    - ���������� ��� PAC Wago 750.@n
/// @c I7186_E - ���������� ��� PAC I7186_E. ������ ��������� ����������
/// ����������� ��� ��������� ���������� ������ � �������� (@ref Getch � @ref
/// Printf), ���� ��������� ��������� �� ������� � ���������� ������.
/// 
/// @par ������� ������:
/// @$Rev:$.\n
/// @$Author:$.\n
/// @$Date::                     $.
#ifndef SYS_H
#define SYS_H

typedef unsigned char       u_char;
typedef unsigned int        u_int;
typedef unsigned long int   u_long;

#include <string.h>
#include <stdio.h>

#ifdef I7186_E
#include "7186e.h"
#endif // I7186_E

#ifdef WIN32
#include <conio.h>

#define Print printf

#ifdef DEBUG
#define Getch getch
#endif // DEBUG

#endif // WIN32

#ifdef W750
#define Print printf

#ifdef DEBUG
int Getch();
#endif // DEBUG

#endif // W750

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
        /// @param buf       - ����� ������, ������ ����� ������������ ������.
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
        //  [ 1 ]
    /// @var total_size
    /// @brief ����� ������ ������.
    //  [ 2 ]
    /// @var available_start_pos
    /// @brief ��������� ��������� �����.
    /// @details ��� �������� ����������������� ��������� �������.
    //  [ 3 ]
    /// @var available_end_pos
    /// @brief �������� ��������� �����.
    /// @details ��� �������� ����������������� ��������� �������.
    u_int total_size;           // 1
    u_int available_start_pos;  // 2
    u_int available_end_pos;    // 3
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
        /// �������� �������� �������������� ����� ����� @ref get_memory_block
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
            MT_EEPROM,
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

        /// @brief ���������� ������������ ��������� ������ ��� ������ � 
        /// �����������.
        ///
        /// @return - ��������� �� ������������ ������ ������ @ref
        /// NV_memory_manager.
        static NV_memory_manager* get_instance();

        static void set_instance( NV_memory_manager* new_instance );
                
    protected:
        /// ����������� ��������� ������ ��� ������ �������.
        static NV_memory_manager *instance;

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
#endif // SYS_H
