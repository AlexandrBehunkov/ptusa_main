/// @file g_device.h
/// @brief �������� �������� �������, ������� ������������ ��������� �
/// ����������� � �������� �� �������.
///
/// ����������� ������� ����������, ������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @c DRIVER   - ���������� ��� �������� � OS Windows.@n
/// @c WIN32    - ���������� ��� PAC � �� Windows.@n
/// @c LINUX_OS - ���������� ��� PAC � �� linux.@n ������ ��������� ����������
/// ����������� ������� ����� ������ (@ref int_2,
/// @ref u_int_4, @ref u_int_2).
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.
#ifndef GENERAL_DEVICE_H
#define GENERAL_DEVICE_H

#ifndef DRIVER
#include "sys.h"
#include "tcp_cmctr.h"
#include <vector>

#if defined LINUX_OS
#include "sys_linux.h"
#endif // defined LINUX_OS

#if defined WIN32
#include "sys_win.h"
#endif // defined WIN32

//-----------------------------------------------------------------------------
/// @brief ��������� ����������, ����������� ��������� ��� � ������ ������.
class i_Lua_save_device    
    {    
    public:
        /// @brief ���������� ������ ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_device( char *buff ) = 0; 

        /// @brief ���������� ������ ������� � �������.
		virtual void print() const = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ��������� ����������, ����������� �������� ������� �� �������.
class i_cmd_device    
	{      
	public:
        /// @brief ���������� �������� �������.
        ///
        /// @param prop [ in ] - ��� ��������.
        /// @param idx [ in ]  - ������ ��� ��������.
        /// @param val [ in ]  - ��������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������.
        virtual int set_cmd( const char *prop, u_int idx, double val ) = 0;

        /// @brief ���������� ��������� �������.
        ///
        /// @param prop [ in ] - ��� ��������.
        /// @param idx [ in ]  - ������ ��� ��������.
        /// @param val [ in ]  - ��������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������.
        virtual int set_cmd( const char *prop, u_int idx, char *val ) = 0;
	};

#endif // DRIVER
//-----------------------------------------------------------------------------
/// @brief ������������ ��������� - �������� ��� ���������� ������ PAC. ������
/// ��� �������� ���������� � ��� � �� ��������� �� ������ (PC).
class device_communicator             
    {   
    public:
        enum CMD
            {
            CMD_GET_INFO_ON_CONNECT = 10, ///< ������ ���. � PAC ����� ���������� �������.

            CMD_GET_DEVICES = 100,   ///< ������ ���. �� ����������� PAC.
            CMD_GET_DEVICES_STATES,  ///< ������ ���. � ��������� ��������� PAC.
            CMD_EXEC_DEVICE_COMMAND, ///< ���������� ������� ��� ����������.

            CMD_GET_PAC_ERRORS,      ///< ������ ���. �� ������� PAC.
            CMD_SET_PAC_ERROR_CMD,   ///< ���������� ������� ��� ������.
            };
#ifdef DRIVER
    };
#else // DRIVER

    private:
        /// ������������ ��������� ������.
        static auto_smart_ptr < device_communicator > instance;

    public:
        /// @brief ��������� ������������� ���������� ������.
        static device_communicator* get_instance()
            {
            static char is_init = 0;
            if ( 0 == is_init )
            	{
                is_init = 1;
                instance = new device_communicator();
            	}

            return instance;
            }

        /// @brief ��������� ������������� ���������� ������.
        static void set_instance( device_communicator* new_instance )
            {
            instance = new_instance;
            }

        enum CONSTANTS
            {
            C_SERVICE_N = 1, ///< ����� ������� �������������.            
            };

        /// @brief ����������, ���������� � ��� � �� ��������� ���������� ��
        /// ������ (PC).
        static std::vector< i_Lua_save_device* > dev;

    public:
        device_communicator()
            {            
            }

        ~device_communicator()
            {
            }

        /// @brief ����� �� ������� ��������� ������.
        void print() const;

        /// @brief ���������� ����������.
        int add_device( i_Lua_save_device *dev );

        /// @brief ������ ��� ������ � device_communicator.
        static long write_devices_states_service( long len, u_char *data,
            u_char *outdata );
    };
//-----------------------------------------------------------------------------
#define G_DEVICE_CMMCTR device_communicator::get_instance()

#endif // DRIVER

#endif // DEVICES_H
