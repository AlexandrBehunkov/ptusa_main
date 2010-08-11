/// @file g_device.h
/// @brief �������� �������� �������, ������� ������������ ��������� �
/// ����������� � �������� �� �������.
///
/// ����������� ������� ����������, ������� ������.
/// ������ ����������:
///   GLOB0V512  - GLOB 0 V 512 - ������ ����� 512 ����� ��������� �����
///   ��������� PAC;
///   TANK1PAR12 - TANK 1 PAR 12 - �������� 1 ����� ����� 1.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @c DRIVER  - ���������� ��� �������� � OS Windows.@n
/// @c PAC     - ���������� ��� PAC.@n
/// @c PAC_W750    - ���������� ��� PAC Wago 750.@n
/// @c I7186_E - ���������� ��� PAC I7186_E.@n
/// @c I7188_E - ���������� ��� PAC I7188_E.@n
/// @c I7188   - ���������� ��� PAC I7188. ������ ��������� ����� ����������� 
/// ���������������� ���������� ����������� ��� ��������� ���������� ������ �
/// ��������, ���� ��������� ��������� �� ������� � ���������� ������. ����� 
/// ��� ���������� ����������� ������� ����� ������ (@ref int_2, @ref u_int_4,
/// @ref u_int_2).
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.
#ifndef GENERAL_DEVICE_H
#define GENERAL_DEVICE_H

#ifndef PAC 
#ifndef DRIVER
#define PAC
#endif // DRIVER
#endif // PAC 

#ifdef DRIVER
#include "CmnHdr.h"
typedef short           int_2;

typedef unsigned int    u_int_4;
typedef unsigned short  u_int_2;
#endif // DRIVER

#ifdef PAC
#include "sys.h"
#include "tcp_cmctr.h"

#if defined LINUX || defined UCLINUX
#include "sys_linux.h"
#endif // defined LINUX || defined UCLINUX

#ifdef PAC_W750
#include "sys_w750.h"
#endif // PAC_W750

#endif // PAC

//-----------------------------------------------------------------------------
/// @brief ����� ��������� �� ������� (��� PAC), ��� � ���� �������� (��� PC).
///
/// @param err_str    - ���������.
/// @param is_need_CR - ���� �������� ������� �� ����� ������.
void print_str( char *err_str, char is_need_CR = 1 );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������, ����������� ��������� ��� � ������ ������.
class i_save_device    
    {    
    public:
        /// @brief ���������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///        
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_state( char *buff ) = 0;

        /// @brief ���������� ����������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_changed_state( char *buff ) = 0;

        /// @brief ���������� ������ ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_device( char *buff ) = 0;      
    };
//-----------------------------------------------------------------------------
#ifdef DRIVER
/// @brief ��������� ����������, ����������� ������� ��� �� ������ ������.
class i_load_device    
    {    
    public:
        /// @brief ���������� ��������� ���������� �� ������.
        ///
        /// @param buff [ out ] - ����� ������, ������ ����������� ������.
        ///
        /// @return >= 0 - ���������� ��������� ����.
        virtual int load_state( char *buff ) = 0;

        /// @brief ���������� ����������� ��������� ���������� �� ������.
        ///
        /// @param buff [ out ] - ����� ������, ������ ����������� ������.
        ///
        /// @return >= 0 - ���������� ��������� ����.
        virtual int load_changed_state( char *buff ) = 0;  

        /// @brief ���������� ������ ���������� �� ������.
        ///
        /// @param buff [ out ] - ����� ������, ������ ����������� ������.
        ///
        /// @return >= 0 - ���������� ��������� ����.
        virtual int load_device( char *buff ) = 0; 
    };
#endif // DRIVER
//-----------------------------------------------------------------------------
/// @brief ��������� ����������, ������� ��������� �������, �����������  �
/// ������ ������.
class i_cmd_device    
    {    
    public:
        /// @brief ���������� �������, ���������� � ������.
        ///
        /// @param buff [ out ] - ����� ������, ������ ����������� ������.
        ///
        /// @return >= 0 - ���������� ��������� ����.
        virtual int parse_cmd( char *buff  ) = 0;    
    };
//-----------------------------------------------------------------------------
/// @brief ��������� ����������, ������� ��������������� ��������� ���������.
///
/// ���������� �������� � �������.
class i_device    
    {    
    public:
#ifdef DRIVER
        /// @brief ��������� ������ ���������� � ������� ���������.
        virtual u_int_4 get_idx() = 0;                  

        /// @brief ������������ ������ ���������� � ������� ���������.
        virtual void set_idx( u_int_4 new_idx ) = 0;
#endif // DRIVER

        /// @brief ���������� ����� ����������.
        virtual u_int_4 get_n() const = 0;

        /// @brief ����� �� ������� ����������.
        virtual void print() const = 0;         
    };
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ����������.
class i_simple_device: public i_device, 
    public i_cmd_device,
#ifdef DRIVER
    public i_load_device,
#endif // DRIVER
    public i_save_device
    {  
    public:
        virtual ~i_simple_device()
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ����������. ��� ������� �� ����� �������
/// ���������.
class i_complex_device: public i_simple_device
    {
    public:

#ifdef DRIVER
        /// ����������, ������� ������ ������ ������� ����������.
        i_complex_device  *compl_dev;  
#endif // DRIVER

        /// ���� ������� ���������.
        enum TYPES
            {
            COMPLEX_DEV,        ///< ������� ����������.
            GROUP_DEV_BYTE,     ///< ������ ��������� � ������������ ������ 1 ����, ��� byte.
            GROUP_DEV_LONG,     ///< -//- 4 �����, ��� unsigned long.
            GROUP_DEV_FLOAT = 4,///< -//- 4 �����, ��� float.
            ARRAY_DEV_BYTE,     ///< ������� ����������. ������ � ���� �������, ��� byte.
            ARRAY_DEV_FLOAT,    ///< -//- ��� float.
            ARRAY_DEV_LONG,     ///< -//- ��� long int.
            ARRAY_DEV_UINT,     ///< -//- ��� unsigned int.

            ARRAY_DEV_INT,      ///< -//- ��� int.

            ARRAY_DEV_STR,      ///< -//- ��� ������.
            ARRAY_DEV_ULONG,    ///< -//- ��� unsigned long int.
            };

        virtual ~i_complex_device()
            {
            }

        virtual char            get_type() const = 0;
        virtual u_int_4         get_n() const = 0;
        virtual const char*     get_name() const = 0;
        virtual u_int_4         get_subdev_quantity() const = 0;
        virtual i_save_device*  get_save_dev( u_int_4 idx ) const = 0;

        virtual int             set_type( char new_type ) = 0;
        virtual int             set_n( u_int_4 new_n ) = 0;
        virtual int             set_name( char * new_name ) = 0;
        virtual int             set_subdev_quantity( u_int_4 new_dev_cnt ) = 0;
#ifdef DRIVER
        virtual i_load_device*  get_load_dev( u_int_4 idx ) = 0;
#endif // DRIVER        
    };
//-----------------------------------------------------------------------------
/// @brief ������� ������� ����������.
class complex_device: public i_complex_device
    {
    public:
        i_simple_device **sub_dev;  ///< �������������.

    protected:
        enum CONSTANTS
            {
            MAX_NAME_LENGTH = 20 ///< ������������ ����� �����.
            };

        u_int_4 n;              ///< ���������� �����.
        u_int_4 sub_dev_cnt;    ///< ���������� ������������.        
        char    name[ MAX_NAME_LENGTH ]; ///< ���.

        char    type;           ///< ���.
        u_int_4 idx;            ///< ����� ���������� ������� ���������.

    public:      
        complex_device();
        complex_device( u_int_4 n, const char *new_name, u_int_2 new_subdev_cnt,
            char type );

        virtual ~complex_device();

#ifdef DRIVER
        complex_device( u_int_4 n, char *new_name, u_int_2 new_subdev_cnt, 
            char type, i_complex_device *owner_compl_dev );
#endif // DRIVER

        char            get_type() const;
        u_int_4         get_n() const;
        const char*     get_name() const;
        u_int_4         get_subdev_quantity() const;
        i_save_device*  get_save_dev( u_int_4 idx ) const;

        /// @brief ����� ���������� @ref i_save_device.
        int save_device( char *buff );

        /// @brief ����� ���������� @ref i_save_device.
        int save_changed_state( char *buff );

        /// @brief ����� ���������� @ref i_save_device.
        int save_state( char *buff );

        int             set_type( char new_type );
        int             set_n( u_int_4 new_n );
        int             set_name( char * new_name );
        int             set_subdev_quantity( u_int_4 new_dev_cnt );
#ifdef DRIVER        
        i_load_device*  get_load_dev( u_int_4 idx );

        i_simple_device* get_sub_dev( u_int_4 id ) const;
        i_complex_device* get_sub_complex_dev( char *sub_dev_name ) const;
#endif // DRIVER        

        int load_device( char *buff );        
        int load_changed_state( char *buff );
        int load_state( char *buff  );

        /// @brief ����� �� ������� ��������� ������.
        void print() const;

        int     parse_cmd( char *buff );
        u_int_4 get_idx();
        void    set_idx( u_int_4 new_idx );
    };
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// @brief ������������ ��������� - �������� ��� ���������� ������ PAC. ������
/// ��� �������� ���������� � ��� � �� ��������� �� ������ (PC).
class device_communicator
#ifdef DRIVER        
    : public i_load_device        
#endif // DRIVER                
    {   
    public:
        enum CMD
            {
            GET_INFO_ON_CONNECT = 10, ///< ������ ���. � PAC ����� ���������� �������.

            GET_DEVICES = 100,
            GET_DEVICES_STATES,
            GET_DEVICES_CHANGED_STATES,
            EXEC_DEVICE_CMD,

            GET_PAC_ERRORS,
            SET_PAC_ERROR_CMD,
            };


#ifdef PAC
    private:
        static u_int dev_cnt;

        /// ������������ ��������� ������.
        static auto_smart_ptr < device_communicator > instance;

    public:
        /// @brief ��������� ������������� ���������� ������.
        static device_communicator* get_instance()
            {
            return instance;
            }

        /// @brief ��������� ������������� ���������� ������.
        static void set_instance( device_communicator* new_instance )
            {
            instance = new_instance;
            }

        enum CONSTANTS
            {
            C_SERVICE_N = 1,
            C_MAX_COMLEX_DEVICES = 40,
            };

        static i_complex_device *dev[ C_MAX_COMLEX_DEVICES ];
#else
        unsigned int     dev_cnt;
        i_complex_device **dev;
#endif // PAC

    public:
        device_communicator();

        ~device_communicator();

        /// @brief ����� �� ������� ��������� ������.
        void print() const;

#ifdef DRIVER
        /// @brief ��������� ���������� �� �����.
        ///
        /// @param dev_name - ��� ����������.
        /// @param dev_n    - ����� ����������.
        /// @param name     -  ��� ������.
        ///
        /// @return > 0 - ��������� �� ������� ����������.
        /// @return   0 - �� ������� ������� ����������.
        i_complex_device* get_group( char* dev_name, u_int_4 dev_n, 
            char* name ) const;

        /// @brief ����� ���������� @ref i_load_device.
        int load_state( char *buff );

        /// @brief ����� ���������� @ref i_load_device.
        int load_changed_state( char *buff );

        /// @brief ����� ���������� @ref i_load_device.
        int load_device( char *buff );        
#endif // DRIVER               


#ifdef PAC
        /// @brief ���������� ����������.
        int add_device( i_complex_device *dev );

        /// @brief ������ ��� ������ � device_communicator.
        static long write_devices_states_service( long len, u_char *data,
            u_char *outdata );
#endif //PAC
    };
//-----------------------------------------------------------------------------
#ifdef PAC
#define G_DEVICE_CMMCTR device_communicator::get_instance()
#endif // PAC

#endif // DEVICES_H
