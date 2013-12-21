/// @file PAC_dev.h
/// @brief ������, ������� ��������� ������� ��������
/// ��������� ��������� PAC �� ������. ����� ����������� ������ ��� ������ �
/// ������������ ( �������, ������,... ). ������ ������������ ������ �
/// ����������� ( PAC ).
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG - ���������� c ������� ���������� ���������� � �������.
/// @c DEBUG_NO_WAGO_MODULES - ������� ���������� �������� ��� �������
/// Wago (��������� ������ � ����).
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef PAC_DEVICES_H
#define PAC_DEVICES_H

#include <string.h>
#include <stdio.h>

#include <vector>
#include <string>

#include "dtime.h"
#include "wago.h"
#include "g_device.h"

#include "param_ex.h"

#include "smart_ptr.h"

//-----------------------------------------------------------------------------
/// @brief ���������� c �����������.
///
/// ��������� �������� � ����������������� ������ (��������� �������� �����
/// ������������ PAC). ������ � ���������� ������������ �� ������ ������ �
/// �������� (�������� ������ ����� �� �����). ������ �������� ����� ���.
class par_device
    {
    friend class device;

    public:
        /// @param par_cnt - ���������� ����������.
        par_device( u_int par_cnt );

        ~par_device();

        /// @brief ���������� � ���� ������� Lua.
        ///
        /// ��������� �� ������ �������� ���������� � ���� �������:
        /// ���_��������� = '��������'.
        ///
        /// @param str - ������, ���� ���������.
        void save_device( char *str );

        /// @brief ���������� ������� (��������� �������� ���������).
        ///
        /// @param name - ��� ������� (��������������� ���������).
        /// @param val - ����� ��������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������, �������� � ����� ������ �� ������.
        int set_cmd( const char *name, double val );

        /// @brief ��������� �������� ���������.
        ///
        /// @param idx - ������ ���������.
        /// @param offset - �������� �������.
        /// @param value - ����� ��������.
        void set_par( u_int idx, u_int offset, float value );

        /// @brief ��������� �������� ���������.
        ///
        /// @param idx - ������ ���������.
        /// @param offset - �������� �������.
        ///
        /// @return �������� ���������.
        float get_par( u_int idx, u_int offset );

        /// @brief ������� ����� ���������.
        ///
        /// @param idx - ������ ���������.
        /// @param offset - �������� �������.
        /// @param name - ��� ���������.
        void set_par_name( u_int idx, u_int offset, const char* name );

        /// @brief ��������� ���������� ��� �������� �������� ������ ����������.
        saved_params_u_int_4* get_err_par() const
            {
            return err_par;
            }

        /// @brief ��������� ���������� ��� �������� �������� ������ ����������.
        void set_err_par( saved_params_u_int_4* err_par )
            {
            this->err_par = err_par;
            }

    protected:
        /// @brief ������ ����������.
        saved_params_u_int_4 *err_par;

        enum CONSTANTS
            {
            C_MAX_PAR_NAME_LENGTH = 20, ///< ������������ ����� ����� ���������.
            };

        saved_params_float *par; ///< ���������.
        char **par_name;         ///< �������� ����������.
    };
//-----------------------------------------------------------------------------
/// @brief ��������� ��������.
class i_counter
    {
    public:
        /// @brief ������������ ������ ��������.
        virtual void pause() = 0;

        /// @brief ������������� ������ ��������.
        virtual void start() = 0;

        /// @brief ����� �������� � ��������� �����.
        ///
        /// ����� ������ ��� ����������� ������ ���������� ������� @ref start().
        virtual void reset() = 0;

        /// @brief ����� �������� � ����������� �����.
        void restart();

        /// @brief ��������� �������� ��������.
        virtual u_int get_quantity() = 0;

        /// @brief ��������� �������� ��������.
        virtual float get_flow() = 0;

        /// @brief ��������� ��������� ������ ��������.
        virtual int get_state() = 0;

        virtual ~i_counter();
    };
//-----------------------------------------------------------------------------
/// @brief ��������� ������������������� ������� (mixproof).
class i_mix_proof
    {
    public:
        enum STATES
            {
            ST_CLOSE = 0,   ///< �������.
            ST_OPEN,        ///< �������.
            ST_UPPER_SEAT,  ///< ������� ������� �����.
            ST_LOWER_SEAT,  ///< ������� ������ �����.
            };

        /// @brief ������� ������� �����.
        virtual void open_upper_seat() = 0;

        /// @brief ������� ������ �����.
        virtual void open_lower_seat() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� �����.
///
/// �������� �����, ���������� ������� � �.�. �������� ��������� �����
/// ���������.
class i_DI_device: public i_cmd_device
    {
    public:
        /// @brief ��������� ��������� ����������.
        ///
        /// @return ��������� ���������� � ���� ������ �����.
        virtual int get_state () = 0;

        /// @brief �������� ��������� ���������.
        ///
        /// ��� ������ - �������� �� ��� ���.
        ///
        /// @return 1 - ���������� �������.
        /// @return 0 - ���������� ���������.
        virtual bool is_active();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� ������.
///
/// ������, ������� � �.�. �������� ��������� ����� ���������.
class i_DO_device: public i_DI_device
    {
    public:
        /// @brief ��������� ���������� � ������ ������� ������.
        virtual void on();

        virtual void print() const = 0;

        /// @brief ���������� ���������� � ������ ������� ������.
        virtual void off() = 0;

        /// @brief ��������� ������ ��������� ���������� � ������ ������� ������.
        ///
        /// @param new_state - ����� ��������� ����������.
        virtual void set_state( int new_state );

    protected:
        /// @brief ��������� ������ ��������� ����������.
        ///
        /// @param new_state - ����� ��������� �������.
        virtual void direct_set_state( int new_state ) = 0;

        /// @brief ��������� ����������.
        ///
        /// ��������� ���������� � �������� ���������. ��� ������� ��� ��������
        /// ��� �������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void direct_on() = 0;

        /// @brief ������� �� ������ �����.
        ///
        /// � ������ ������ ���������� ����������� �� �������� �� �������
        /// (����������� ��������� �� ������ �� ����������).
        ///
        /// @return true - ������ ����� �������.
        /// @return false - ������ ����� ��������.
        virtual bool get_manual_mode() const = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� �� ������ ����������� �����.
///
/// �����������, ������ � �.�. �������� ��������� ����� ���������.
class i_AI_device: public i_cmd_device
    {
    public:
        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @return - ������� ��������� ���������� � ���� �������� �����.
        virtual float get_value() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� ������.
///
/// ���������� ����� ���������� � �.�. �������� ��������� ����� ���������.
class i_AO_device: public i_AI_device
    {
    public:
        /// @brief ���������� ���������� � ������ ������� ������.
        virtual void off() = 0;

        /// @brief ��������� �������� ��������� ���������� � ������ ������� ������.
        virtual void set_value( float new_value );

    protected:
        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @param new_value - ����� ��������� ����������.
        virtual void direct_set_value( float new_value ) = 0;

        /// @brief ������� �� ������ �����.
        ///
        /// � ������ ������ ���������� ����������� �� �������� �� �������
        /// (����������� ��������� �� ������ �� ����������).
        ///
        /// @return true - ������ ����� �������.
        /// @return false - ������ ����� ��������.
        virtual bool get_manual_mode() const = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ��������� ���������� ��� � �����������, ��� � ����������� ��������.
class i_DO_AO_device: public i_AO_device, public i_DO_device
    {
    public:
        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ����� �������������� �������� ����������, ������� ������������ �
/// �������.
class device : public i_DO_AO_device, public par_device
    {
    public:
        /// @brief ���������� �������.
        ///
        /// ������ � �������� ������� �������� �����.
        ///
        /// @param prop - �������� �������.
        /// @param idx  - ������ ��������.
        /// @param val  - ��������.
        virtual int set_cmd( const char *prop, u_int idx, double val );

        /// @brief ���������� �������.
        ///
        /// ������ � �������� ������� ������.
        ///
        /// @param prop - �������� �������.
        /// @param idx  - ������ ��������.
        /// @param val  - ��������.
        virtual int set_cmd( const char *prop, u_int idx, char *val );

        /// @brief ���������� ���������� � ���� ������� Lua.
        ///
        /// @param prefix - ������� ����� ������� ������� (������ ������
        /// ��������� - ��� ����������� �������������� ������).
        /// @param buff [out] - ����� ������ ������.
        virtual int save_device( char *buff, const char *prefix );

#ifdef RM_PAC
        /// @brief ���������� ��������� ���������� � ���� ������� Lua.
        ///
        /// @param prefix - ������� ����� ������� ������� (������ ��������
        /// �������).
        /// @param buff [out] - ����� ������ ������.
        virtual int rm_save_device_state( char *buff, const char *prefix );
#endif // RM_PAC

        enum CONSTANTS
            {
            C_DEVICE_TYPE_CNT = 17,     ///< ���������� ����� ���������.

            C_MAX_NAME = 10
            };

        static const char DEV_NAMES[][ 5 ];

        /// ���� ���������.
        enum DEVICE_TYPE
            {
            DT_NONE = -1,      ///< ��� �� ���������.

            DT_V = 0,   ///< ������.
            DT_VC,      ///< ����������� ������.
            DT_M,       ///< ���������.
            DT_LS,      ///< ������� (����/���).
            DT_TE,      ///< �����������.
            DT_FS,      ///< ������ (����/���).
            DT_GS,      ///< ������ ���������.
            DT_FQT,     ///< �������.
            DT_LT,      ///< ������� (��������).
            DT_QT,      ///< ������������.

            DT_HA,      ///< ��������� �������� ������������.
            DT_HL,      ///< ��������� �������� ������������.
            DT_SB,      ///< ������.
            DT_DI,      ///< ���������� ������� ������.
            DT_DO,      ///< ���������� �������� ������.
            DT_AI,      ///< ���������� ������� ������.
            DT_AO,      ///< ���������� �������� ������.
            };

        /// ������� ���������.
        enum DEVICE_SUB_TYPE
            {
            DST_NONE = -1,      ///< ������ �� ���������.

            //V
            DST_V_DO1 = 1,      ///< ������ � ����� ������� ����������.
            DST_V_DO2,          ///< ������ � ����� �������� ����������.
            DST_V_DO1_DI1_FB_OFF,  ///< ������ � ����� ������� ���������� � ����� �������� ������ �� �������� ���������.
            DST_V_DO1_DI1_FB_ON,   ///< ������ � ����� ������� ���������� � ����� �������� ������ �� �������� ���������.
            DST_V_DO1_DI2,      ///< ������ � ����� ������� ���������� � ����� ��������� �������.
            DST_V_DO2_DI2,      ///< ������ � ����� �������� ���������� � ����� ��������� �������.
            DST_V_MIXPROOF,     ///< ������ ������������������.
            DST_V_AS_MIXPROOF,  ///< ������ � ����� �������� ���������� � ����� ��������� ������� � AS ����������� (������������������).

            //LS
            DST_LS_MIN = 1,     ///< ����������� �� ����� �������.
            DST_LS_MAX,         ///< ����������� �� ����� ��������.

            //M,
            DST_M = 1,          ///< ����� ��� ���������� �������� ��������.
            DST_M_FREQ,         ///< ����� � ����������� �������� ��������.
            DST_M_REV, 	        ///< ����� � �������� � ����������� �������� ��������.
            DST_M_REV_FREQ,     ///< ����� � �������� ��� ���������� �������� ��������.
            };

        device( const char *dev_name, device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt );

        virtual ~device();

        const char *get_name() const
            {
            return name;
            }

        const char *get_description() const
            {
            return description;
            }

        void set_descr( const char *new_description );

        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void direct_off() = 0;

        /// @brief ���������� ���������� � ������ ������� ������.
        void off();

        /// @brief ����� ������� � �������.
        ///
        /// ��� ������������� � ���������� �����.
        virtual void print() const;

        /// @brief ��������� ����������� ������ ����������.
        ///
        /// @return - ����� ����������.
        virtual u_int_4 get_serial_n() const
            {
            return s_number;
            }

        /// @brief ��������� ����������� ������ ����������.
        ///
        /// @return - ����� ����������.
        void set_serial_n( u_int_4 s_n )
            {
            s_number = s_n;
            }

        /// @brief ��������� ���� ����������.
        int get_type() const
            {
            return type;
            }

        /// @brief ��������� ������� ����������.
        int get_sub_type() const
            {
            return sub_type;
            }

        /// @brief ��������� �������������� �������, �������� ������� -
        /// ����������.
        ///
        /// ��� ������������� � Lua.
        virtual void set_property( const char* field, device* dev )
            {
            }

    protected:
        /// @brief ���������� �������������� ������ ���������� � ���� ������� Lua.
        ///
        /// @param buff [out] - ����� ������ ������.
        virtual void save_device_ex( char *buff )
            {
            }

        u_int_4 s_number;            ///< ���������������� ����� ����������.

        DEVICE_TYPE     type;        ///< ��� ����������.
        DEVICE_SUB_TYPE sub_type;    ///< ������ ����������.

        bool get_manual_mode() const
            {
            return is_manual_mode;
            }

    private:
        bool is_manual_mode;      ///< ������� ������� ������.

        char name[ C_MAX_NAME ];
        char *description;
    };
//-----------------------------------------------------------------------------
/// @brief ����������� ����������.
///
/// ���������� ��� ����������� ���������� ������ ���������� � ��������������
/// �������. ������ ������� ������ ������ �� ������.
class dev_stub : public device,
    public i_counter
    {
    public:
        dev_stub(): device( 0, DT_NONE, DST_NONE, 1 )
            {
            }

        u_int_4 get_serial_n() const;
        void    print() const;

        float   get_value();
        void    direct_set_value( float new_value );

        void    direct_on();
        void    direct_off();
        void    direct_set_state( int new_state );
        int     get_state();

        void    pause();
        void    start();
        void    reset();
        u_int   get_quantity();
        float   get_flow();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ����� ��� ��������� ���������� ���������.
class digital_wago_device : public device,
    public wago_device
    {
    public:
        digital_wago_device( const char *dev_name, device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt );

        virtual ~digital_wago_device();

        float   get_value();
        void    direct_set_value( float new_value );
        void    direct_set_state( int new_state );

#ifdef DEBUG_NO_WAGO_MODULES
        /// @brief ��������� ��������� �������.
        ///
        /// @return - ��������� �������.
        int  get_state();

        void direct_on();
        void direct_off();
#endif // DEBUG_NO_WAGO_MODULES

        void    print() const;

#ifdef DEBUG_NO_WAGO_MODULES
    protected:
        char state;  ///< ��������� ����������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ����� ��� ��������� ���������� ���������.
class analog_wago_device : public device, public wago_device
    {
    public:
        analog_wago_device( const char *dev_name,
            device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type,
            u_int par_cnt ):
        device( dev_name, type, sub_type, par_cnt ),
        wago_device( dev_name )
#ifdef DEBUG_NO_WAGO_MODULES
            ,value( 0 )
#endif  // DEBUG_NO_WAGO_MODULES
            {
            }

        void  direct_set_state( int new_state );
        int   get_state();

        void  print() const;
        void  direct_on();
        void  direct_off();

#ifdef DEBUG_NO_WAGO_MODULES
        float get_value();
        void  direct_set_value( float new_value );

#else  // DEBUG_NO_WAGO_MODULES

        float get_value() = 0;

#endif // DEBUG_NO_WAGO_MODULES


#ifdef DEBUG_NO_WAGO_MODULES
    private:
        float value;    ///< ��������� ����������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ������, �����, ����� ����������...
class DO1 : public digital_wago_device
    {
    public:
        DO1( const char *dev_name, device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type ):
        digital_wago_device( dev_name, type, sub_type, 0 )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state();
        void direct_on();
        void direct_off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� ��������.
///
/// ��� ����� ���� ������, �����...
class DO2 : public digital_wago_device
    {
    public:
        DO2( const char *dev_name, device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt ):
        digital_wago_device( dev_name, type, sub_type, par_cnt )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state();
        void direct_on();
        void direct_off();

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0, ///< ������ ������ ����������� ������ �1.
            DO_INDEX_2,     ///< ������ ������ ����������� ������ �2.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ��������� �������.
///
class valve: public digital_wago_device
    {
    public:
        /// @param is_on_fb - ������������ �������� ����� �� ���������� ���������.
        /// @param is_off_fb - ������������ �������� ����� �� ����������� ���������.
        /// @param number - ����� ����������.
        /// @param type - ��� ����������.
        /// @param sub_type - ������ ����������.
        valve( bool is_on_fb, bool is_off_fb,
            const char *dev_name, device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type );

        /// @param number - ����� ����������.
        /// @param type - ��� ����������.
        /// @param sub_type - ������ ����������.
        valve( const char *dev_name, device::DEVICE_TYPE type, device::DEVICE_SUB_TYPE sub_type ):
            digital_wago_device( dev_name, type, sub_type, 0 ),
            is_on_fb( false ),
            is_off_fb( false ),
            start_switch_time( 0 )
            {
            }

        /// @brief ��������� �������� �������� ����� �� ���������� ���������.
        virtual int get_on_fb_value()
            {
            return on_fb;
            }

        /// @brief ��������� �������� �������� ����� �� ����������� ���������.
        virtual int get_off_fb_value()
            {
            return off_fb;
            }

        /// @brief ���������� �������������� ������.
        void save_device_ex( char *buff );

        //��������� ������� (�����������).
        enum VALVE_STATE_EX
            {
            VX_ON_FB_OFF  = 11,  ///< �������, �� ���������.
            VX_OFF_FB_OFF = 10,  ///< ��������, �� ���������.

            VX_UPPER_SEAT_FB_OFF = 32, ///< ������� ������� �����, �� ���������.
            VX_LOWER_SEAT_FB_OFF = 33, ///< ������� ������ �����, �� ���������.

            VX_UPPER_SEAT = 2,   ///< ������� ������� �����.
            VX_LOWER_SEAT = 3,   ///< ������� ������ �����.

            VX_ON_FB_OK  = 1,    ///< �������, �� ��.
            VX_OFF_FB_OK = 0,    ///< ��������, �� ��.

            VX_ON_FB_ERR  = -2,  ///< �������, ������ ��.
            VX_OFF_FB_ERR = -3,  ///< ��������, ������ ��.

            //���� ������� ������.
            VX_ON_FB_OFF_MANUAL  = 31,   ///< ������� �������, �� ���������.
            VX_OFF_FB_OFF_MANUAL = 30,   ///< �������� �������, �� ���������.

            VX_LOWER_SEAT_MANUAL = 23,   ///< ������� ������� ������ �����.
            VX_UPPER_SEAT_MANUAL = 22,   ///< ������� ������� ������� �����.

            VX_ON_FB_OK_MANUAL  = 21,    ///< ������� �������, �� ��.
            VX_OFF_FB_OK_MANUAL = 20,    ///< �������� �������, �� ��.

            VX_ON_FB_ERR_MANUAL  = -12,  ///< ������� �������, ������ ��.
            VX_OFF_FB_ERR_MANUAL = -13,  ///< �������� �������, ������ ��.


            VX_UNKNOWN = 100,            ///< ����������� ���������.
            };

        /// @brief ��������� ������������ ��������� ������� (���� ��������
        /// �����, ������� ������, ...).
        int get_state()
#if _MSC_VER >= 1700
        final;
#else
        ;
#endif

#ifdef DEBUG_NO_WAGO_MODULES
        int set_cmd( const char *prop, u_int idx, double val );
#endif // DEBUG_NO_WAGO_MODULES

        //��������� ��� ���������� ��������� ������������ ��������� � ������
        // ���� ��������� (������ �����, �������� �����, ...).
    protected:

        ///��������� ������� ��� ����� �������� �����.
        enum VALVE_STATE
            {
            V_LOWER_SEAT = 3, ///< ������� ������ �����.
            V_UPPER_SEAT = 2, ///< ������� ������� �����.

            V_ON  = 1,        ///< �������.
            V_OFF = 0,        ///< ��������.
            };

        /// @brief ��������� ��������� ������� ��� ����� �������� �����.
        virtual VALVE_STATE get_valve_state() = 0;

        /// @brief ��������� ��������� �������� �����.
        virtual bool get_fb_state()
            {
            return true;
            }

    protected:
        enum FB_STATE
            {
            FB_IS_AND_OFF = 0, ///< �������� ����� ���������.
            FB_IS_AND_ON,      ///< �������� ����� ��������.
            };

        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 2,  ///���������� �������������� ����������.

            P_ON_TIME = 1,
            P_FB
            };

    private:
        /// @brief ���� �������� ����� �� ���������� ���������.
        bool is_on_fb;

        /// @brief ���� �������� ����� �� ����������� ���������.
        bool is_off_fb;

        bool on_fb;
        bool off_fb;

    protected:
        u_long start_switch_time;
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ���������� ������� � ����� ���������� ������.
///
class valve_DO1_DI1_off : public valve
    {
    public:
        valve_DO1_DI1_off( const char *dev_name );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.
            DI_INDEX = 0,           ///< ������ ������ ����������� �����.
            };

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        void direct_on();
        void direct_off();

#endif // DEBUG_NO_WAGO_MODULES

        // ��������� valve ��� ���������� ��������� ������������ ��������� �
        // ������ ���� ��������� (������ �����, �������� �����, ...).
    protected:
        VALVE_STATE get_valve_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return ( VALVE_STATE ) digital_wago_device::get_state();
#else
            int o = get_DO( DO_INDEX );

            return ( VALVE_STATE ) o;
#endif // DEBUG_NO_WAGO_MODULES
            }

        bool get_fb_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return valve::get_fb_state();
#else
            int o = get_DO( DO_INDEX );
            int i = get_DI( DI_INDEX );

            if ( o != i )
                {
                start_switch_time = get_millisec();
                return true;
                }

            if ( get_delta_millisec( start_switch_time ) < get_par( valve::P_ON_TIME, 0 ) )
                {
                return true;
                }

            return false;
#endif // DEBUG_NO_WAGO_MODULES
            }

#ifndef DEBUG_NO_WAGO_MODULES
        int get_off_fb_value()
            {
            return get_DI( DI_INDEX );
            }

        int get_on_fb_value()
            {
            return false;
            }
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ���������� ������� � ����� ���������� ������.
///
class valve_DO1_DI1_on : public valve
    {
    public:
        valve_DO1_DI1_on( const char *dev_name ): valve( true, false,
            dev_name, DT_V, DST_V_DO1_DI1_FB_ON )
            {
            }

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.
            DI_INDEX = 0,           ///< ������ ������ ����������� �����.
            };

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        void direct_on();
        void direct_off();
#endif // DEBUG_NO_WAGO_MODULES

        //��������� ��� ���������� ��������� ������������ ��������� � ������
        // ���� ��������� (������ �����, �������� �����, ...).
    protected:
        VALVE_STATE get_valve_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return ( VALVE_STATE ) digital_wago_device::get_state();
#else
            int o = get_DO( DO_INDEX );

            return ( VALVE_STATE ) !o;
#endif // DEBUG_NO_WAGO_MODULES
            }

        bool get_fb_state()
            {
            int o = get_DO( DO_INDEX );
            int i = get_DI( DI_INDEX );

            if ( o == i )
                {
                start_switch_time = get_millisec();
                return true;
                }

            if ( get_delta_millisec( start_switch_time ) < get_par( valve::P_ON_TIME, 0 ) )
                {
                return true;
                }

            return false;
            }

#ifndef DEBUG_NO_WAGO_MODULES
        int get_on_fb_value()
            {
            return get_DI( DI_INDEX );
            }

        int get_off_fb_value()
            {
            return false;
            }
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� ��������� �������.
///
class valve_DO1_DI2 : public valve
    {
    public:
        valve_DO1_DI2( const char *dev_name ):
          valve( true, true, dev_name, DT_V, DST_V_DO1_DI2 )
            {
            }

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.

            DI_INDEX_1 = 0,         ///< ������ �1 ������ ����������� �����.
            DI_INDEX_2,             ///< ������ �2 ������ ����������� �����.
            };

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        void direct_on();
        void direct_off();
#endif // DEBUG_NO_WAGO_MODULES

        //��������� ��� ���������� ��������� ������������ ��������� � ������
        // ���� ��������� (������ �����, �������� �����, ...).
    protected:
        VALVE_STATE get_valve_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return ( VALVE_STATE ) digital_wago_device::get_state();
#else
            int o = get_DO( DO_INDEX );

            return ( VALVE_STATE ) o;
#endif // DEBUG_NO_WAGO_MODULES
            }

        bool get_fb_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return valve::get_fb_state();
#else
            int o = get_DO( DO_INDEX );
            int i1 = get_DI( DI_INDEX_1 );
            int i0 = get_DI( DI_INDEX_2 );

            if ( ( o == 0 && i0 == 1 && i1 == 0 ) ||
                ( o == 1 && i1 == 1 && i0 == 0 ) )
                {
                return true;
                }

            if ( get_delta_millisec( start_switch_time ) < get_par( valve::P_ON_TIME, 0 ) )
                {
                return true;
                }

            return false;
#endif // DEBUG_NO_WAGO_MODULES
            }

#ifndef DEBUG_NO_WAGO_MODULES
        int get_off_fb_value()
            {
            return get_DI( DI_INDEX_1 );
            }

        int get_on_fb_value()
            {
            return get_DI( DI_INDEX_2 );
            }
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ���������� � ����� ��������� �������.
///
class valve_DO2_DI2 : public valve
    {
    public:
        valve_DO2_DI2( const char *dev_name ):
            valve( true, true, dev_name, DT_V, DST_V_DO2_DI2 )
            {
            }

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0,         ///< ������ �1 ������ ����������� ������.
            DO_INDEX_2,             ///< ������ �2 ������ ����������� ������.

            DI_INDEX_1 = 0,         ///< ������ �1 ������ ����������� �����.
            DI_INDEX_2,             ///< ������ �2 ������ ����������� �����.
            };

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        void direct_on();
        void direct_off();

#endif // DEBUG_NO_WAGO_MODULES

        //��������� ��� ���������� ��������� ������������ ��������� � ������
        // ���� ��������� (������ �����, �������� �����, ...).
    protected:
        VALVE_STATE get_valve_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return ( VALVE_STATE ) digital_wago_device::get_state();
#else
            int o1 = get_DO( DO_INDEX_2 );

            return ( VALVE_STATE ) o1;
#endif // DEBUG_NO_WAGO_MODULES
            }

        bool get_fb_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return true;
#else
            int o0 = get_DO( DO_INDEX_1 );
            int o1 = get_DO( DO_INDEX_2 );
            int i0 = get_DI( DI_INDEX_1 );
            int i1 = get_DI( DI_INDEX_2 );

            if ( o1 == i1 && o0 == i0 )
                {
                return true;
                }

            if ( get_delta_millisec( start_switch_time ) < get_par( valve::P_ON_TIME, 0 ) )
                {
                return true;
                }

            return false;
#endif // DEBUG_NO_WAGO_MODULES
            }

#ifndef DEBUG_NO_WAGO_MODULES
        int get_off_fb_value()
            {
            return get_DI( DI_INDEX_1 );
            }

        int get_on_fb_value()
            {
            return get_DI( DI_INDEX_2 );
            }
#endif // DEBUG_NO_WAGO_MODULES

    };
//-----------------------------------------------------------------------------
/// @brief ������ mixproof.
class valve_mix_proof : public i_mix_proof,  public valve
    {
    public:
        valve_mix_proof( const char *dev_name
            ): valve( true, true, dev_name, DT_V, DST_V_MIXPROOF )
            {
            }

        /// @brief ������� ������� �����.
        void open_upper_seat();

        /// @brief ������� ������ �����.
        void open_lower_seat();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            DO_INDEX_U,     ///< ������ ������ ����������� ������ �������� �����.
            DO_INDEX_L,     ///< ������ ������ ����������� ������ ������� �����.

            DI_INDEX_OPEN = 0, ///< ������ ������ ����������� ����� ������.
            DI_INDEX_CLOSE,     ///< ������ ������ ����������� ����� ������.
            };

         void direct_set_state( int new_state );

#ifndef DEBUG_NO_WAGO_MODULES
        void direct_on();
        void direct_off();
#endif // DEBUG_NO_WAGO_MODULES

        //��������� ��� ���������� ��������� ������������ ��������� � ������
        // ���� ��������� (������ �����, �������� �����, ...).
    protected:
        VALVE_STATE get_valve_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return ( VALVE_STATE ) digital_wago_device::get_state();
#else
            int o = get_DO( DO_INDEX );

            if ( o == 0 && get_DO( DO_INDEX_U ) == 1 ) return V_UPPER_SEAT;
            if ( o == 0 && get_DO( DO_INDEX_L ) == 1 ) return V_LOWER_SEAT;

            return ( VALVE_STATE ) o;
#endif // DEBUG_NO_WAGO_MODULES
            }

        bool get_fb_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return true;
#else
            int o = get_DO( DO_INDEX );
            int i0 = get_DI( DI_INDEX_CLOSE );
            int i1 = get_DI( DI_INDEX_OPEN );

            if ( ( o == 0 && i0 == 1 && i1 == 0 ) ||
                ( o == 1 && i1 == 1 && i0 == 0 ) )
                {
                return true;
                }

            if ( o == 0 && get_DO( DO_INDEX_L ) == 1 ) return true;
            if ( o == 0 && get_DO( DO_INDEX_U ) == 1 ) return true;

            if ( get_delta_millisec( start_switch_time ) < get_par( valve::P_ON_TIME, 0 ) )
                {
                return true;
                }

            return false;
#endif // DEBUG_NO_WAGO_MODULES
            }

#ifndef DEBUG_NO_WAGO_MODULES
        int get_off_fb_value()
            {
            return get_DI( DI_INDEX_CLOSE );
            }

        int get_on_fb_value()
            {
            return get_DI( DI_INDEX_OPEN );
            }
#endif // DEBUG_NO_WAGO_MODULES

    };
//-----------------------------------------------------------------------------
/// @brief ������ AS-mixproof.
class valve_AS_mix_proof : public device, public i_mix_proof
    {
    public:
        valve_AS_mix_proof( const char *dev_name );

        void open_upper_seat();
        void open_lower_seat();

        float get_value();

        void direct_set_state( int new_state );

        void direct_set_value( float new_value );

        void direct_off();

        void direct_on();

        int get_state();

    private:
#ifdef DEBUG_NO_WAGO_MODULES
        STATES state;  ///< ��������� ����������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �����������, ������ (��������)...
class AI1 : public analog_wago_device
    {
    public:
        AI1( const char *dev_name, device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt, u_int *start_par_idx );

    protected:

        enum CONSTANTS
            {
            P_ZERO_ADJUST_COEFF = 1,    ///< ����� ����.

            ADDITIONAL_PARAM_COUNT = 1, ///< ���������� ����������.

            C_AI_INDEX = 0,             ///< ������ ������ ����������� �����.
            };

        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val()
            {
            return 0;
            }

        /// @brief ��������� ������������ �������� ������ ����������.
        virtual float get_min_val()
            {
            return 0;
            }
#ifdef DEBUG_NO_WAGO_MODULES
        float get_value();
#endif // DEBUG_NO_WAGO_MODULES

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        float get_value();
        void  direct_set_value( float new_value );

#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief �����������.
class temperature_e : public AI1
    {
    public:
        temperature_e( const char *dev_name ): AI1( dev_name, DT_TE, DST_NONE, 0, 0 )
            {
            }

        float get_value()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            float v = analog_wago_device::get_value();

            return -1000 == v ? -1000 : AI1::get_value();
#else
            float v = get_AI( C_AI_INDEX, 0, 0 );
            return -1000 == v ? -1000 :
                get_par( P_ZERO_ADJUST_COEFF, 0 ) + v;
#endif
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������� �������.
class level_e : public AI1
    {
    public:
        level_e( const char *dev_name ): AI1( dev_name, DT_TE, DST_NONE, 0, 0 )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������������.
class concentration_e : public AI1
    {
    public:
        concentration_e( const char *dev_name ): AI1( dev_name, DT_QT, DST_NONE,
            ADDITIONAL_PARAM_COUNT, &start_param_idx )
            {
            set_par_name( P_MIN_V,  start_param_idx, "P_MIN_V" );
            set_par_name( P_MAX_V,  start_param_idx, "P_MAX_V" );
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAM_COUNT = 2,

            P_MIN_V = 1,   ///< ������ ��������� ������������ ��������.
            P_MAX_V,       ///< ������ ��������� ������������� ��������.
            };

        u_int start_param_idx;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ����������� �����.
class analog_input : public AI1
    {
    public:
        analog_input( const char *dev_name ): AI1( dev_name, DT_AI, DST_NONE,
            ADDITIONAL_PARAM_COUNT, &start_param_idx )
            {
            set_par_name( P_MIN_V,  start_param_idx, "P_MIN_V" );
            set_par_name( P_MAX_V,  start_param_idx, "P_MAX_V" );
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAM_COUNT = 2,

            P_MIN_V = 1,   ///< ������ ��������� ������������ ��������.
            P_MAX_V,       ///< ������ ��������� ������������� ��������.
            };

        u_int start_param_idx;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ����������� ������...
class AO1 : public analog_wago_device
    {
    public:
        AO1( const char *dev_name,
            device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type,
            u_int par_cnt ):
        analog_wago_device( dev_name, type, sub_type, par_cnt )
            {
            }

        virtual float get_min_value() = 0;
        virtual float get_max_value() = 0;

#ifndef DEBUG_NO_WAGO_MODULES
        float get_value();
        void  direct_set_value( float new_value );
#endif // DEBUG_NO_WAGO_MODULES

    protected:
        enum CONSTANTS
            {
            AO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� ����������� ������...
class analog_output : public AO1
    {
    public:
        analog_output( const char *dev_name ) :
            AO1( dev_name, DT_AO, DST_NONE, ADDITIONAL_PARAM_COUNT )
              {
              }

          float get_min_value()
              {
              return get_par( P_MIN_VALUE, 0 );
              }

          float get_max_value()
              {
              return get_par( P_MAX_VALUE, 0 );
              }

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAM_COUNT = 2,

            P_MIN_VALUE = 1,   ///< ������ ��������� ������������ ��������.
            P_MAX_VALUE,       ///< ������ ��������� ������������� ��������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ����������� ������.
class analog_valve : public AO1
    {
    public:
        analog_valve( const char *dev_name ): AO1( dev_name, DT_VC, DST_NONE, 0 )
            {
            }

        float get_min_value()
            {
            return C_MIN;
            }

        float get_max_value()
            {
            return C_MAX;
            }

    private:
        enum CONSTANTS
            {
            C_MIN = 0,   ///< ����������� ��������.
            C_MAX = 100, ///< ������������ ��������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �������� �����, ������ (����/���)...
class DI1 : public digital_wago_device
    {
    public:
        DI1( const char *dev_name,
            device::DEVICE_TYPE type,
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt ):
        digital_wago_device( dev_name, type, sub_type, par_cnt )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        void direct_on();
        void direct_off();

        int get_state()
            {
            return get_DI( DI_INDEX );
            }

    private:
        enum CONSTANTS
            {
            DI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ����������.
class valve_DO1 : public valve
    {
    public:
        valve_DO1( const char *dev_name ) : valve( dev_name, DT_V, DST_V_DO1 )
            {
            }

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        void direct_on();
        void direct_off();

#endif // DEBUG_NO_WAGO_MODULES

    protected:
        /// @brief ��������� ��������� ������� ��� ����� �������� �����.
        VALVE_STATE get_valve_state()
            {
#ifdef DEBUG_NO_WAGO_MODULES
            return ( VALVE_STATE ) digital_wago_device::get_state();
#else
            return ( VALVE_STATE ) get_DO( DO_INDEX );
#endif // DEBUG_NO_WAGO_MODULES
            };

        /// @brief ��������� ��������� �������� �����.
        bool get_fb_state()
            {
            return true;
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ����������.
class valve_DO2 : public DO2
    {
    public:
        valve_DO2( const char *dev_name ): DO2( dev_name, DT_V, DST_V_DO2, 0 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������������� (�������, �����).
class motor : public device, public wago_device
    {
    public:
        motor( const char *dev_name, device::DEVICE_SUB_TYPE sub_type ):
            device( dev_name, DT_M, sub_type, ADDITIONAL_PARAM_COUNT ),
            wago_device( dev_name )
#ifdef DEBUG_NO_WAGO_MODULES
            ,state( 0 ),
            freq( 0 )
#endif // DEBUG_NO_WAGO_MODULES
            {
            set_par_name( P_ON_TIME,  0, "P_ON_TIME" );
            }

        void save_device_ex( char *buff );

        float get_value();

        void direct_set_value( float value );

        void direct_set_state( int new_state );

        int  get_state();

        void direct_on();

        void direct_off();

    private:
        u_int start_param_idx;

        enum CONSTANTS
            {
            ADDITIONAL_PARAM_COUNT = 1,

            C_MIN_VALUE = 0,
            C_MAX_VALUE = 100,

            P_ON_TIME = 1,    ///< ������ ��������� ������� ��������� (����).

            DO_INDEX = 0,         ///< ������ ������ ����������� ������.
            DO_INDEX_REVERSE = 1, ///< ������ ������ ����������� ������ �������.

            DI_INDEX = 0,     ///< ������ ������ ����������� �����.

            AO_INDEX = 0,     ///< ������ ������ ����������� ������.
            };

        u_long start_switch_time;

#ifdef DEBUG_NO_WAGO_MODULES
        char  state;  ///< ��������� ����������.

        float freq;   ///< ��������� ���������� (�������).
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� ������.
class level_s : public DI1
    {
    public:
        level_s( const char *dev_name, device::DEVICE_SUB_TYPE sub_type );

        bool is_active();

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 1,

            P_DT = 1,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� �������.
class flow_s : public DI1
    {
    public:
        flow_s( const char *dev_name ): DI1( dev_name, DT_FS, DST_NONE,
            ADDITIONAL_PARAMS_COUNT )
            {
            set_par_name( P_DT,  0, "P_DT" );
            }

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 1,

            P_DT = 1,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ���������.
class state_s : public DI1
    {
    public:
        state_s( const char *dev_name ): DI1( dev_name, DT_GS, DST_NONE,
            ADDITIONAL_PARAMS_COUNT )
            {
            set_par_name( P_DT,  0, "P_DT" );
            }

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 1,

            P_DT = 1,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ����������� ����� �����.
class DI_signal : public DI1
    {
    public:
        DI_signal( const char *dev_name ): DI1( dev_name, DT_DI, DST_NONE, 0 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������.
class button : public DI1
    {
    public:
        button( const char *dev_name ): DI1( dev_name, DT_SB, DST_NONE, 0 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ������ ����������.
class DO_signal : public DO1
    {
    public:
        DO_signal( const char *dev_name ): DO1( dev_name, DT_DO, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������.
class siren : public DO1
    {
    public:
        siren( const char *dev_name ): DO1( dev_name, DT_HA, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������.
class lamp : public DO1
    {
    public:
        lamp( const char *dev_name ): DO1( dev_name, DT_HL, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief �������.
class counter : public device,
    public i_counter,
    public wago_device
    {
    public:
        counter( const char *dev_name ): device(
            dev_name, DT_FQT, DST_NONE, ADDITIONAL_PARAMS_COUNT ),
            wago_device( dev_name ),
            value( 0 ),
            last_read_value( 0 ),
            state( S_STOP ),
            flow_value( 0 )
            {
            set_par_name( P_MIN_FLOW,  0, "P_MIN_FLOW" );
            set_par_name( P_MAX_FLOW,  0, "P_MAX_FLOW" );
            set_par_name( P_CZ,        0, "P_CZ" );
            set_par_name( P_DT,        0, "P_DT" );
            }

        virtual ~counter()
            {
            }

        float get_value();
        void  direct_set_value( float new_value );
        int   get_state();
        void  direct_on();
        void  direct_off();
        void  direct_set_state( int new_state );
        void  print() const;

        void  pause();
        void  start();
        void  reset();
        u_int get_quantity();
        float get_flow();

        void set_property( const char* field, device* dev );


        int set_cmd( const char *prop, u_int idx, double val )
            {
            switch ( prop[ 0 ] )
                {
            case 'F':
                flow_value = ( float ) val;
                break;

            default:
                return device::set_cmd( prop, idx, val );
                }

            return 0;
            }

        //Lua.
        void save_device_ex( char *buff )
            {
            sprintf( buff, "F=%.2f, ", get_flow() );
            }

    private:
        enum STATES
            {
            S_STOP,
            S_WORK,
            S_PAUSE,

            S_ERROR = -1,
            };

        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 4,

            P_MIN_FLOW = 1,
            P_MAX_FLOW,
            P_CZ,
            P_DT,

            AI_FLOW_INDEX = 0,  ///< ������ ������ ����������� ����� (�����).
            AI_Q_INDEX    = 1,  ///< ������ ������ ����������� ����� (�����).

            MAX_VAL = 65535L,   ///< ������������ �������� ��������.
            };

        u_int value;
        u_int last_read_value;

        STATES state;

        float flow_value;

        std::vector < device* > motors;
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������.
///
/// �������� ���������� ��� ���� ����������� �������.
class device_manager: public i_Lua_save_device
    {
    public:
        device_manager();

        virtual ~device_manager();

        /// @brief ��������� ����������.
        device* get_device( int dev_type, const char *dev_name );

        /// @brief ��������� ����������.
        device* get_device( u_int serial_dev_n )
            {
            if ( serial_dev_n < project_devices.size() )
            	{
                return project_devices[ serial_dev_n ];
            	}

            return &stub;
            }

        /// @brief ��������� ������� �� ��� ������.
        i_DO_device* get_V( const char *dev_name );

        /// @brief ��������� ������������ ������� �� ������.
        i_AO_device* get_VC( const char *dev_name );

        /// @brief ��������� ��������� �� ������.
        i_DO_AO_device* get_M( const char *dev_name );

        /// @brief ��������� ������ �� ������.
        i_DI_device* get_LS( const char *dev_name );

        /// @brief ��������� ������� �� ������.
        i_DI_device* get_FS( const char *dev_name );

        /// @brief ��������� ������� ��������� �� ������.
        i_DI_device* get_GS( const char *dev_name );

        /// @brief ��������� ����������� ����� �� ������.
        i_AI_device* get_AI( const char *dev_name );

        /// @brief ��������� ����������� ������ �� ������.
        i_AO_device* get_AO( const char *dev_name );

        /// @brief ��������� �������� �� ������.
        i_counter* get_FQT( const char *dev_name );

        /// @brief ��������� ����������� �� ������.
        i_AI_device* get_TE( const char *dev_name );

        /// @brief ��������� �������� ������ �� ������.
        i_AI_device* get_LT( const char *dev_name );

        /// @brief ��������� �������� ����� �� ������.
        i_DI_device* get_DI( const char *dev_name );

        /// @brief ��������� ������ �� ������.
        i_DI_device* get_SB( const char *dev_name );

        /// @brief ��������� ������������ ������ �� ������.
        i_DO_device* get_DO( const char *dev_name );

        /// @brief ��������� �������� ������������.
        i_DO_device* get_HA( const char *dev_name );

        /// @brief ��������� �������� ������������.
        i_DO_device* get_HL( const char *dev_name );

        /// @brief ��������� ������� ������������ �� ������.
        i_AI_device* get_QT( const char *dev_name );

        /// @brief ��������� ������������� ���������� ������.
        static device_manager* get_instance();

        /// @brief ���������� ������ ������� � �������.
        void print() const;

        const char* get_name_in_Lua() const
            {
            return "Device manager";
            }

        /// @brief ��������� ���������� ����������.
        dev_stub* get_stub()
            {
            return &stub;
            }

        int init_params();

#ifdef __BORLANDC__
#pragma option -w-inl
#endif // __BORLANDC__
        int save_device( char *buff );

#ifdef RM_PAC
        int rm_save_device( char *buff );

        int rm_save_device_state( char *buff );
#endif // RM_PAC

#ifdef __BORLANDC__
#pragma option -w.inl
#endif // __BORLANDC__

        //int save_params_as_Lua_str( char* str )
        //    {
        //    str[ 0 ] = 0;

        //    for ( u_int i = 0; i < project_devices.size(); i++ )
        //        {
        //        project_devices[ i ]->save_params_as_Lua_str( str + strlen( str ) );
        //        }

        //    return 0;
        //    }

    protected:
        dev_stub stub;  ///< ����������-��������, ��������� ����������.

        struct range    ///< �������� ��������� ������ ����.
            {
            int start_pos;
            int end_pos;
            };

        /// ��������� ��������� ���� �����.
        range dev_types_ranges[ device::C_DEVICE_TYPE_CNT ];

        int get_device_n( device::DEVICE_TYPE dev_type,
            const char *dev_name );

        std::vector< device* > project_devices; ///< ��� ����������.

        /// @brief ������������ ��������� ������.
        static auto_smart_ptr < device_manager > instance;

    public:
        // @brief ���������� ����������.
        //
        // ���������� �� Lua.
        wago_device* add_wago_device( int dev_type, int dev_sub_type,
            const char *dev_name, char * descr );
    };
//-----------------------------------------------------------------------------
/// @brief ������.
///
/// ������������� ���������������� �������.
class timer
    {
    public:
        enum STATE
            {
            S_STOP = 0, ///< �� ��������.
            S_WORK,     ///< ��������.
            S_PAUSE,    ///< �����.
            };

        /// @brief ���������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        int save( char *buff );

        /// @brief ���������� ��������� ���������� �� ������.
        ///
        /// @param buff [ out ] - ����� ������, ������ ����� ����������� ������.
        ///
        /// @return >= 0 - ���������� ��������� ����.
        int load( char *buff );

        /// @brief ��������� ������� ������ ��� ���������� ��������� ����������.
        ///
        /// @return >= 0 - ������ ������, ����.
        int get_saved_size() const;

        timer();

        /// @brief ������ �������.
        void start();

        /// @brief ����� �������.
        void reset();

        /// @brief ����� �������.
        void pause();

        /// @brief �������� ������ ������� �������.
        ///
        /// @return 1 - ����� �����.
        /// @return 0 - ����� �� �����.
        bool is_time_up() const;

        /// @brief ��������� ������� ������ �������.
        ///
        /// @return - ����� ������ �������.
        u_long  get_work_time() const;

        /// @brief ��������� ������� �������.
        ///
        /// @param new_countdown_time - �������.
        void set_countdown_time( u_long new_countdown_time );

        /// @brief ��������� ������� �������.
        ///
        /// @return - ������� �������.
        u_long  get_countdown_time() const;

        /// @brief ��������� ��������� �������.
        ///
        /// @return - ��������� �������.
        STATE get_state() const;

    private:
        u_long  last_time;  ///< �����, ����� ������ ��� �������/����������.
        u_long  work_time;  ///< ����� ������ �������.

        STATE   state;           ///< ���������.
        u_long  countdown_time;  ///< ������� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������.
///
/// ���������� ������ ��������.
class timer_manager
    {
    public:
        /// @param timers_count - ���������� �������� � ������.
        timer_manager( u_int timers_count );

        ~timer_manager();

        /// @brief ���������� ��������� ������� �� �������.
        ///
        /// @param index - ������ �������.
        ///
        /// @return - ������ � ������ ��������, �������� - � ������ ������ ��
        /// ��������.
        timer* operator[] ( unsigned int index );

        /// @brief ���������� ��������.
        int get_count() const
            {
            return timers_cnt;
            }

    private:
        u_int   timers_cnt; ///< ���������� ��������.
        timer   *timers;    ///< �������.

        timer   stub;       ///< ��������.
    };
//-----------------------------------------------------------------------------
///@brief ��������� ��������� ���������.
///
///@return �������� ��������� �������.
device_manager* G_DEVICE_MANAGER();
//-----------------------------------------------------------------------------
/// @brief ��������� ������� �� ������.
///
/// @param number - ����� �������.
/// @return - ������ � �������� �������. ���� ��� ������ �������, ������������
/// �������� (@ref dev_stub).
i_DO_device* V( u_int dev_n );
i_DO_device* V( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ������������ ������� �� ������.
///
/// @param number - ����� �������.
/// @return - ������ � �������� �������. ���� ��� ������ �������, ������������
/// �������� (@ref dev_stub).
i_AO_device* VC( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ��������� �� ������.
///
/// @param number - ����� ���������.
/// @return - ��������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_AO_device* M( u_int dev_n );
i_DO_AO_device* M( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������ �� ������.
///
/// @param number - ����� ����������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* LS( u_int dev_n );
i_DI_device* LS( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������� �� ������.
///
/// @param number - ����� ����������� �������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* FS( u_int dev_n );
i_DI_device* FS( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ����� �� ������.
///
/// @param number - ����� ����������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* AI( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������ �� ������.
///
/// @param number - ����� ����������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AO_device* AO( u_int dev_n );
i_AO_device* AO( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� �� ������.
///
/// @param number - ����� ��������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_counter* FQT( u_int dev_n );
i_counter* FQT( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� �� ������.
///
/// @param number - ����� �����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* TE( u_int dev_n );
i_AI_device* TE( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������ �� ������.
///
/// @param number - ����� �������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* LT( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ��������� �� ������.
///
/// @param number - ����� ������� ���������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* GS( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������ �� ������.
///
/// @param number - �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* HA( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������ �� ������.
///
/// @param number - �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* HL( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ �� ������.
///
/// @param number - �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* SB( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ����� �� ������.
///
/// @param number - ����� �������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* DI( u_int dev_n );
i_DI_device* DI( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ ���������� �� ������.
///
/// @param number - ����� ������ ����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* DO( u_int dev_n);
i_DO_device* DO( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ������������ �� ������.
///
/// @param number - ����� ������� ������������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* QT( u_int dev_n );
i_AI_device* QT( const char *dev_name );
//-----------------------------------------------------------------------------
/// @brief ��������� ������������ ����������.
///
/// @return - ����������� ����������.
dev_stub* STUB();
//-----------------------------------------------------------------------------
/// @brief ��������� ���������� �� ������ � ����.
///
/// @param type   - ��� ����������.
/// @param s_number - ���������� ����� ����������.
/// @return - ����������.
device* DEVICE( int s_number );
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
