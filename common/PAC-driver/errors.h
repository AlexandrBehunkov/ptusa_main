/// @file errors.h
/// @brief �������� �������� ������������, ������� ������������ � 
///  �������� �� ������� ��� ����������� �������������� �� ������.
///
/// ����� @ref base_error, �������� ��� ����������� ������ � �������, ��������   
/// ��� ����������� ����������. ��� �������� ���� ������ ������ �����
/// @ref dev_errors_manager. 
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c PAC    - ���������� ��� �����������.@n
/// @c DRIVER - ���������� ��� �������� (�� Windows).
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef ERRORS_H
#define ERRORS_H

#if !( defined PAC_PC || defined PAC_WAGO_750_860 || defined UPAC_7186E ) && \
    !defined DRIVER
#error You must define type!
#endif

#if ( defined PAC_PC || defined PAC_WAGO_750_860 || defined UPAC_7186E ) && \
    !defined PAC
#define PAC
#endif

//-----------------------------------------------------------------------------
#if defined PAC || defined DRIVER
enum ALARM_STATE
    {
    AS_NORMAL,
    AS_ALARM,
    AS_RETURN, 
    AS_ACCEPT, 
    };

enum PRIORITY_TYPE
    {
    PT_SYSTEM,
    PT_CRITICAL,
    PT_IMPOTENT,
    PT_UNIMPOTENT,
    P_INFORMATIONAL,    
    };

enum ALARM_TYPE
    {
    AT_DISCRETE,
    AT_VALUE,
    AT_DEVIATION,
    AT_RATE_OF_CHANGE,
    AT_SPECIAL,
    };


enum OBJECT_TYPE      ///< ��� �������, ��� �������� �������� �������.
    {
    OT_UNKNOWN,
    OT_PAC,           ///< PAC.
    };

enum ALARM_CLASS      ///< ����� �������.
    {
    AC_UNKNOWN,
    AC_NO_CONNECTION, ///< ������ �����.

    AC_COM_DRIVER,    ///< ������ ������ � COM-������.
    AC_RUNTIME_ERROR, ///< ������ �� ����� ������.
    };

enum ALARM_SUBCLASS         ///< �������� �������.
    {
    //AC_NO_CONNECTION,     ///< ������ �����.
    AS_WAGO = 1,            ///< ������ ������� WAGO.
    AS_PANEL,               ///< ������ ������� EasyView.
    AS_MODBUS_DEVICE,       ///< ������ ����������, ������������� �� Modbus.

    AS_EASYSERVER = 5,      ///< ������ EasyServer.

    //AC_RUNTIME_ERROR,     ///< ������ �� ����� ������.
    AS_EMERGENCY_BUTTON = 1,///< ������ ��������� ������.
    };

#endif // defined PAC || defined WIN32

extern const char *ALARM_CLASS_STR[ 4 ];
extern const char *ALARM_SUBCLASS_STR[ 4 ][ 5 ];
extern const int ALARM_CLASS_PRIORITY[ 4 ];

//-----------------------------------------------------------------------------
#ifdef PAC

#include "errors.h"
#include "param_ex.h"
//#include "sys.h"
#include "smart_ptr.h"
#include "PAC_dev.h"

//-----------------------------------------------------------------------------
/// @brief ������� ����� � ����������� �� ������ ����������.
///
class base_error
    {
    public:
        base_error();

        /// @brief ����� ���������� ������ � �������� �� ��������� (0).
        virtual void reset_errors_params() = 0;

        /// @brief ���������� ������ � ����� ��� �������� �� ������.
        ///        
        /// @param stream       - ����� ����.
        /// @param is_new_state - ���������� �� ��������� ������ 
        ///                       (0 - ���, 1 - ��).
        ///
        /// @return - ���������� ���������� ����.        
        virtual int save_to_stream( u_char *stream, char &is_new_state ) = 0;

        /// @brief ���������� ����� ����������� ������ � �������.
        virtual void print() const = 0;

        /// @brief ��������� ���� �������.
        virtual unsigned char get_type() const = 0;

        /// @brief ��������� ������ �������.
        virtual unsigned int get_n() const = 0;

        /// @brief ��������� ������ ������ �������.
        virtual unsigned int get_object_alarm_n() const = 0;

        /// @brief ���������� ������� ��� �������.
        virtual int set_cmd( int cmd ) = 0;

    protected:
        unsigned char error_state;    ///< C�������� ������.

        enum BASE_ERRORS_CMD          ///< ���������.
            {            
            BE_CMD_ACCEPT   = 100,    ///< ����������� ������.
            BE_CMD_SUPPRESS = 200,    ///< �������� ������.
            BE_CMD_UNSET_SUPPRESS,    ///< ������ ���������� ������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� �� ������ �������� ���������� (������,
/// �����...).
///
/// � �������� ���������� ����� ���� ������ ���� ������ (������ ��������
/// �����).
class simple_error: public base_error
    {
    public:
        simple_error( device* simple_device = 0 );
        virtual ~simple_error();

        /// @brief ����� ���������� ������ � �������� �� ��������� (0).
        void reset_errors_params();

        /// @brief ���������� ������ � ����� ��� �������� �� ������.
        ///        
        /// @param stream       - ����� ����.
        /// @param is_new_state - ���������� �� ��������� ������ 
        ///                       (0 - ���, 1 - ��).
        ///
        /// @return - ���������� ���������� ����.        
        int save_to_stream( u_char *stream, char &is_new_state );

        /// @brief ���������� ����� ����������� ������ � �������.
        void print() const;

        /// @brief ��������� ���� �������.
        unsigned char get_type() const;

        /// @brief ��������� ������ �������.
        unsigned int get_n() const;

        /// @brief ��������� ������ ������ �������.
        unsigned int get_object_alarm_n() const;

        /// @brief ���������� ������� ��� �������.
        int set_cmd( int cmd );

    private:
        device          *simple_device; ///< ������� ����������.

        enum SYMPLE_ERROR_CONST     ///< ���������.
            {
            SE_ERROR_CODE = 1,      ///< ��� ������ - ��� �������� �����.
            SE_PRIORITY = 300,      ///< ��������� ������ �������� ����������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� �� ���� ������� ������� ���������.
class dev_errors_manager
    {
    public:
        ~dev_errors_manager();
        
        int is_any_error() const;
            
        /// @brief ���������� ���� ������ � ����� ��� �������� �� ������.
        ///        
        /// @param stream - ����� ����.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int save_to_stream( u_char *stream );

        /// @brief ���������� ������ � ������ ������.
        ///        
        /// @param s_error - ����������� ������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_error( base_error *s_error );

        /// @brief ����� ���������� ���� ������ � �������� �� ��������� (0).
        void reset_errors_params();

        /// @brief ���������� ����� ����������� ������ � �������.
        void print();

        /// @brief ��������� ���������� ������.
        void set_cmd( unsigned int cmd, unsigned int object_type, 
            unsigned int object_number, unsigned int object_alarm_number );

        /// @brief ��������� ������������� ���������� ������.
        static dev_errors_manager* get_instance();

    private:

        /// ������������ ��������� ������.
        static auto_smart_ptr < dev_errors_manager > instance;

        /// @param s_errors_cnt - ���������� ������.
        dev_errors_manager();

        enum DEM_CONST
            {
            DEM_MAX_ERRORS_CNT = 29,
            };

        u_int_2 errors_id;

        std::vector< base_error* > s_errors_vector;    ///< ������ ������.

        u_char is_any_err;  //< ������� ������� ������ ������� ���������.
    };
//-----------------------------------------------------------------------------
#define G_DEV_ERRORS_MANAGER dev_errors_manager::get_instance()
//-----------------------------------------------------------------------------
#endif // PAC 

#endif //ERRORS_H
