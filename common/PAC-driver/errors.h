/// @file errors.h
/// @brief �������� �������� ������� � ��������, ������� ������������ � 
///  �������� �� ������� ��� ����������� �������������� �� ������.
///
/// ����� @ref base_error, �������� ��� ����������� ������ � �������, ��������   
/// ��� ����������� ����������. ��� �������� ���� ������ ������ �����
/// @ref dev_errors_manager. 
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c PAC   - ���������� ��� �����������.@n
/// @c WIN32 - ���������� ��� Windows.
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef ERRORS_H
#define ERRORS_H

#ifdef DRIVER
typedef unsigned char uchar;

#include <vector>
#include "SWMRG.h"

#include "PAC_err.h"
#endif // DRIVER
//-----------------------------------------------------------------------------
#if ( defined PAC || defined PAC_PC ) || defined WIN32
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
#ifdef DRIVER

//  ���������� ������������� �������.
struct alarm_id
    {
    int object_type;
    int object_number;
    int object_alarm_number;
    };

#pragma pack( push, 8 ) //������������ ����� �������� �� 8 ����.
//-----------------------------------------------------------------------------
struct alarm_params 
    {
    double  param1;
    double  param2;
    double  param3;
    double  param4;
    double  param5;
    double  param6;
    double  param7;
    double  param8;
    double  param9;
    double  param10;
    };
//-----------------------------------------------------------------------------
struct alarm
    {
    alarm_params params;

    //  ���������� ��� �������:
    //     atDiscrete      - ���������� ( true/false )
    //     atValue         - �������� �������� ( Lo/LoLo, Hi/HiHi )
    //     atDeviation     - ���������� ( MinValue/MajValue )
    //     atRateOfChange  - ��������� �������� ( speed )
    //     atSpecial       - �����������
    ALARM_TYPE type;

    //  �������� �������.
    char *description;

    //  ���������� ������� �� �����
    //  ��������������.
    uchar enable;

    //  ���������� �������������� �������
    //  ����� ���� ������ ������.
    char *group;

    //  ���������� ������� �� ����� ������.
    uchar  inhibit;

    //  ��������� ������� ( 0 - 999 )
    //  ��� �������:
    //     0       - ���������
    //     1-249   - �����������
    //     250-499 - ������
    //     500-749 - ����������
    //     750-999 - ��������������
    int priority;

    //  ��������� �������:
    //     asNormal    - ������� ���
    //     asAlarm     - ������� ����
    //     asReturn    - �������������� ��������
    //                ��������� � ����������
    //                ���������
    //     asAccept    - ������� ������������
    ALARM_STATE state;

    //  ���������� ������� ���������.
    uchar  suppress;

    //  ���������� ������������� �������:
    alarm_id id;

    // id ��������.
    uchar driver_id;

    bool operator == ( const alarm &alarm2 )
        {
        return !strcmp( this->description, alarm2.description ); 
        }

    bool operator < ( const alarm &alarm2 ) const
        {
        return strcmp( this->description, alarm2.description ) < 0 ? 1 : 0;
        }

    alarm( alarm const& copy );

    alarm & operator = ( const alarm & copy );

    int load_from_stream_as_simple_error( char *stream );

    alarm();

    ~alarm();

    void accept()
        {
        state = AS_ACCEPT;
        }
    };
//-----------------------------------------------------------------------------
struct all_alarm
    {
    int     cnt;
    alarm   *alarms; 
    int     id;
    };
//-----------------------------------------------------------------------------
struct error_cmd
    {
    int cmd; 
    int object_type;
    int object_number;
    int object_alarm_number;
    };
//-----------------------------------------------------------------------------
#pragma pack( pop )
//-----------------------------------------------------------------------------
class alarm_manager
    {
    public:

        enum AM_CONST
            {
            AM_MAX_COUNT = 256,
            };

        enum CRITICAL_ERRORS
            {
            CE_WAGO = 1,
            CE_PANEL,
            CE_EMERGENCY_BUTTON,
            };

        alarm_manager();
        ~alarm_manager();

        int add_no_PAC_connection_error( const char *description, 
            uchar project_id );
        int remove_no_PAC_connection_error( const char *description,
            uchar driver_id );

        int get_alarms( uchar driver_id, all_alarm &project_alarms );

        int add_PAC_critical_errors( CRITICAL_ERRORS type, uchar project_id, 
            const char* PAC_name, unsigned int param_number, u_int priority );

        int clear_PAC_critical_errors( uchar driver_id );

        int clear_PAC_simple_devices_errors( uchar driver_id );

        int add_alarm( alarm &new_alarm );

#ifndef MONITORING_DRIVER
    private:
#endif // MONITORING_DRIVER
        //1 - ��� ������������ ������.
        //2 - ������ ������ ��������.   
        //3 - ������ ������ ��� �������� �������.
        //4 - ������ �������� ������������� ������ � ��������.
        //5 - ��� ������ ������ ����� � �������� WAGO.
        unsigned int id[ AM_MAX_COUNT ];                        //1              
        std::vector< alarm > alarms_vector[ AM_MAX_COUNT ];     //2
        alarm   *all_projects_alarms_array[ AM_MAX_COUNT ];     //3
        CSWMRG  synch_all_projects_alarms[ AM_MAX_COUNT ];      //4
        
        static const char *ERRORS_GROUP_WAGO_CONNECT;           //5
        static const char *ERRORS_GROUP_SIMPLE_DEVICES;         //6
        static const char *ERRORS_GROUP_PANEL_CONNECT;          //7
        static const char *ERRORS_GROUP_EMERGENCY_BUTTONS;      //8

        int remove_alarm( alarm &r_alarm );
        int clear( uchar driver_id );

        //������������� ������, ����������� � ������� alarms_vector � ������� 
        //all_projects_alarms_array.
        int synch_project_alarms( uchar driver_id );
    };
#endif // DRIVER

#if defined PAC || defined PAC_PC

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
#endif // PAC || PAC_PC

#endif //ERRORS_H
