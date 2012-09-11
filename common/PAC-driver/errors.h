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

#if !( defined PAC_PC || defined PAC_WAGO_750_860 || defined UPAC_7186E || \
    defined UPAC_5000 ) && \
    !defined DRIVER
#error You must define type!
#endif

#if ( defined PAC_PC || defined PAC_WAGO_750_860 || defined UPAC_7186E || \
    defined UPAC_5000 ) && \
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

#endif // defined PAC || defined WIN32

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

        /// @brief ���������� ������ � ����� ��� �������� �� ������.
        ///        
        /// @param stream       - ����� ����.
        /// @param is_new_state - ���������� �� ��������� ������ 
        ///                       (0 - ���, 1 - ��).
        ///
        /// @return - ���������� ���������� ����.        
        virtual int save_as_Lua_str( char *str, bool &is_new_state ) = 0;        

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

        /// @brief ����� ���������� ������ � �������� �� ��������� (0).
        void reset_errors_params()
            {
            err_par[ P_PARAM_N ] = 0;
            }

    protected:
        saved_params_u_int_4 err_par;

        unsigned char error_state;    ///< C�������� ������.

        enum PARAMS  ///< ��������� ������, ���������� ������ �����.
            {
            P_PARAM_N = 0,	  //����� ���������.

            P_IS_ENABLE = 1,  ///< ���������� ������� �� ����� ��������������.    
            P_IS_INHIBIT = 2, ///< ���������� ������� �� ����� ������.    
            P_IS_SUPPRESS = 4,///< ���������� ������� ���������.
            };

        enum COMMANDS                ///< ���������.
            {     

            C_CMD_ACCEPT   = 100,    ///< ����������� ������.
            C_CMD_SUPPRESS = 200,    ///< �������� ������.
            C_CMD_UNSET_SUPPRESS,    ///< ������ ���������� ������.
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

        /// @brief ���������� ������ � ����� ��� �������� �� ������.
        ///        
        /// @param stream       - ����� ����.
        /// @param is_new_state - ���������� �� ��������� ������ 
        ///                       (0 - ���, 1 - ��).
        ///
        /// @return - ���������� ���������� ����.        
        int save_as_Lua_str( char *str, bool &is_new_state );

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
        int save_as_Lua_str( char *str, u_int_2 &id );

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
        u_int_2 errors_id; // C�������� ������.

        /// ������������ ��������� ������.
        static auto_smart_ptr < dev_errors_manager > instance;

        /// @param s_errors_cnt - ���������� ������.
        dev_errors_manager();

        enum DEM_CONST
            {
            DEM_MAX_ERRORS_CNT = 29,
            };

        std::vector< base_error* > s_errors_vector;    ///< ������ ������.
    };
//-----------------------------------------------------------------------------
#define G_DEV_ERRORS_MANAGER dev_errors_manager::get_instance()
//-----------------------------------------------------------------------------
#endif // PAC 

#endif //ERRORS_H
