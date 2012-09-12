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
/// wago (��������� ������ � ����).
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

    protected:
        enum CONSTANTS
            {
            C_MAX_PAR_NAME_LENGTH = 20, ///< ������������ ����� ����� ���������.
            };

        saved_params_float par; ///< ���������.
        char **par_name;        ///< �������� ����������.
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

        enum CONSTANTS
            {
            C_DEVICE_TYPE_CNT = 17,     ///< ���������� ����� ���������.
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
            DST_V_MIXPROOF,     ///< ������ ��������.
            DST_V_AS_MIXPROOF,  ///< ������ � ����� �������� ���������� � ����� ��������� ������� � AS ����������� (��������).

            //LS
            DST_LS_MIN = 1,     ///< ����������� �� ����� �������.
            DST_LS_MAX,         ///< ����������� �� ����� ��������.

            //M,       
            DST_M_IS_FREQ,      ///< ���� ������� ��������.
            DST_M_NO_FREQ,      ///< ��� ������� ��������.
            };

        device( int number, device::DEVICE_TYPE type, 
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

        void set_name( const char *name, const char *description )
            {
            this->name = new char[ strlen( name ) ];
            strcpy( this->name, name );

            this->description = new char[ strlen( description ) ];
            strcpy( this->description, description );
            }

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
        void print() const;

        /// @brief ��������� ������ ����������.
        ///
        /// @return - ����� ����������.
        u_int_4 get_n() const
            {
            return number;
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

        u_int_4 number;              ///< ����� ����������.

        DEVICE_TYPE     type;        ///< ��� ����������.
        DEVICE_SUB_TYPE sub_type;    ///< ������ ����������.

        bool get_manual_mode() const
            {
            return is_manual_mode;
            }

    private:
        bool is_manual_mode;      ///< ������� ������� ������.

        char *name;
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

        u_int_4 get_n() const;                
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
        digital_wago_device( int number, device::DEVICE_TYPE type, 
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
        analog_wago_device( int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type,
            u_int par_cnt ): 
        device( number, type, sub_type, par_cnt )
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
        DO1( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): 
        digital_wago_device( number, type, sub_type, 0 )
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
        DO2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt ):
        digital_wago_device( number, type, sub_type, par_cnt )
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
class fb_device: public digital_wago_device
    {
    public:
        /// @param is_on_fb - ������������ �������� ����� �� ���������� ���������.
        /// @param is_off_fb - ������������ �������� ����� �� ����������� ���������.
        /// @param number - ����� ����������.
        /// @param type - ��� ����������.
        /// @param sub_type - ������ ����������.
        fb_device( bool is_on_fb, bool is_off_fb, 
            int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type );
        
        /// @brief ��������� �������� �������� ����� �� ���������� ���������.
        virtual int get_on_fb();

        /// @brief ��������� �������� �������� ����� �� ����������� ���������.
        virtual int get_off_fb();

        /// @brief ���������� �������������� ������.
        void save_device_ex( char *buff );

    protected:
        enum FB_STATES
            {
            FB_NO = -1,        ///< ��� �������� �����.

            FB_IS_AND_OFF = 0, ///< �������� ����� ���������.
            FB_IS_AND_ON,      ///< �������� ����� ��������.
            };

        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 3,  ///���������� �������������� ����������.

            P_ON_TIME = 0,
            P_FB_OFF,
            P_FB_ON,            
            };

    private:
        /// @brief ���� �������� ����� �� ���������� ���������.
        bool is_on_fb;

        /// @brief ���� �������� ����� �� ����������� ���������.
        bool is_off_fb;
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ���������� ������� � ����� ���������� ������.
///
class valve_DO1_DI1_off : public fb_device
    {
    public:
        valve_DO1_DI1_off( int number );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.
            DI_INDEX = 0,           ///< ������ ������ ����������� �����.
            };

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state();
        void direct_on();
        void direct_off();

    private:
        int get_off_fb() 
            {
            return get_DI( DI_INDEX );
            }

        u_long start_switch_time;  ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ���������� ������� � ����� ���������� ������.
///
class valve_DO1_DI1_on : public fb_device
    {
    public:
        valve_DO1_DI1_on( int number ): fb_device( true, false, 
            number, DT_V, DST_V_DO1_DI1_FB_ON )
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
        int  get_state();
        void direct_on();
        void direct_off();

    private:
        int get_on_fb() 
            {
            return get_DI( DI_INDEX );
            }

        u_long start_switch_time;  ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� ��������� �������.
///
class valve_DO1_DI2 : public fb_device
    {
    public:
        valve_DO1_DI2( int number ):
          fb_device( true, true, number, DT_V, DST_V_DO1_DI2 )           
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
        int  get_state();
        void direct_on();
        void direct_off();

    private:
        int get_off_fb() 
            {
            return get_DI( DI_INDEX_1 );
            }

        int get_on_fb() 
            {
            return get_DI( DI_INDEX_2 );
            }

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ���������� � ����� ��������� �������.
///
class valve_DO2_DI2 : public fb_device
    {
    public:
        valve_DO2_DI2( int number ):
        fb_device( true, true, number, DT_V, DST_V_DO2_DI2 )        
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
        int  get_state();
        void direct_on();
        void direct_off();

    private:
        int get_off_fb() 
            {
            return get_DI( DI_INDEX_1 );
            }

        int get_on_fb() 
            {
            return get_DI( DI_INDEX_2 );
            }

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ mixproof.
class valve_mix_proof : public i_mix_proof,  public fb_device
    {
    public:
        valve_mix_proof( u_int number 
            ): fb_device( true, true, number, DT_V, DST_V_MIXPROOF )             
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

            DI_INDEX_U = 0, ///< ������ ������ ����������� ����� �������� �����.
            DI_INDEX_L,     ///< ������ ������ ����������� ����� ������� �����.
            };

#ifdef DEBUG_NO_WAGO_MODULES
        void direct_set_state( int new_state );
#endif // DEBUG_NO_WAGO_MODULES

#ifndef DEBUG_NO_WAGO_MODULES
        int  get_state();
        void direct_on();
        void direct_off();
        void direct_set_state( int new_state );

    private:
        int get_off_fb() 
            {
            return get_DI( DI_INDEX_U );
            }

        int get_on_fb() 
            {
            return get_DI( DI_INDEX_L );
            }

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ AS-mixproof.
class valve_AS_mix_proof : public device, public i_mix_proof
    {
    public:
        valve_AS_mix_proof( u_int number );

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
        AI1( u_int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt, u_int *start_par_idx );

    protected:

        enum CONSTANTS
            {
            P_ZERO_ADJUST_COEFF = 0,    ///< ����� ����.

            ADDITIONAL_PARAM_COUNT = 1, ///< ���������� ����������.

            C_AI_INDEX = 0,             ///< ������ ������ ����������� �����.
            };

        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ������ ����������.
        virtual float get_min_val() = 0;

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
        temperature_e( u_int number ): AI1( number, DT_TE, DST_NONE, 0, 0 )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������� �������.
class level_e : public AI1
    {
    public:
        level_e( u_int number ): AI1( number, DT_TE, DST_NONE, 0, 0 )
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
        concentration_e( u_int number ): AI1( number, DT_QT, DST_NONE, 
            ADDITIONAL_PARAM_COUNT, &start_param_idx )
            {
            set_par_name( P_MIN_V,  0, "P_MIN_V" );
            set_par_name( P_MAX_V,  0, "P_MAX_V" ); 
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAM_COUNT = 2,

            P_MIN_V = 0,   ///< ������ ��������� ������������ ��������.
            P_MAX_V,       ///< ������ ��������� ������������� ��������.
            };     

        u_int start_param_idx;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ����������� �����.
class analog_input : public AI1
    {
    public:
        analog_input( u_int number ): AI1( number, DT_AI, DST_NONE, 
            ADDITIONAL_PARAM_COUNT, &start_param_idx )
            {
            set_par_name( P_MIN_V,  0, "P_MIN_V" );
            set_par_name( P_MAX_V,  0, "P_MAX_V" );            
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAM_COUNT = 2,

            P_MIN_V = 0,   ///< ������ ��������� ������������ ��������.
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
        AO1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type,
            u_int par_cnt ): 
        analog_wago_device( number, type, sub_type, par_cnt )
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
        analog_output( u_int number ) : 
          AO1( number, DT_AO, DST_NONE, ADDITIONAL_PARAM_COUNT )
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

            P_MIN_VALUE = 0,   ///< ������ ��������� ������������ ��������.
            P_MAX_VALUE,       ///< ������ ��������� ������������� ��������.
            };         
    };
//-----------------------------------------------------------------------------
/// @brief ����������� ������.
class analog_valve : public AO1
    {
    public:
        analog_valve( u_int number ): AO1( number, DT_VC, DST_NONE, 0 )            
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
        DI1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type, u_int par_cnt ):
        digital_wago_device( number, type, sub_type, par_cnt )
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
class valve_DO1 : public DO1
    {
    public:
        valve_DO1( u_int number ) : DO1( number, DT_V, DST_V_DO1 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ����������.
class valve_DO2 : public DO2
    {
    public:
        valve_DO2( u_int number ): DO2( number, DT_V, DST_V_DO2, 0 )
            {
            }

    protected:
        //Lua.
        void save_device_ex( char *buff )
            {
            sprintf( buff, "P_FB_ON=0, P_FB_OFF=0, " );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������������� (�������, �����).
class motor : public device, public wago_device    
    {
    public:
        motor( u_int number, device::DEVICE_SUB_TYPE sub_type ):            
            device( number, DT_M, sub_type, ADDITIONAL_PARAM_COUNT )
#ifdef DEBUG_NO_WAGO_MODULES
            ,state( 0 ),
            freq( 0 )
#endif // DEBUG_NO_WAGO_MODULES                
            {
            set_par_name( P_ON_TIME,  0, "P_ON_TIME" );
            }

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
            
            P_ON_TIME = 0,

            DO_INDEX = 0,   ///< ������ ������ ����������� ������.

            DI_INDEX = 0,   ///< ������ ������ ����������� �����.

            AO_INDEX = 0,   ///< ������ ������ ����������� ������.
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
        level_s( u_int number, device::DEVICE_SUB_TYPE sub_type );

        bool is_active();

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 1, 

            P_DT = 0,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� �������.
class flow_s : public DI1
    {
    public:
        flow_s( u_int number ): DI1( number, DT_FS, DST_NONE, 
            ADDITIONAL_PARAMS_COUNT )
            {    
            set_par_name( P_DT,  0, "P_DT" );
            }

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 1, 

            P_DT = 0,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ���������.
class state_s : public DI1
    {
    public:
        state_s( u_int number ): DI1( number, DT_GS, DST_NONE, 
            ADDITIONAL_PARAMS_COUNT )
            {     
            set_par_name( P_DT,  0, "P_DT" );
            }

    private:
        enum CONSTANTS
            {
            ADDITIONAL_PARAMS_COUNT = 1, 

            P_DT = 0,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ����������� ����� �����.
class DI_signal : public DI1
    {
    public:
        DI_signal( u_int number ): DI1( number, DT_DI, DST_NONE, 0 )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������.
class button : public DI1
    {
    public:
        button( u_int number ): DI1( number, DT_SB, DST_NONE, 0 )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ������ ����������.
class DO_signal : public DO1
    {
    public:
        DO_signal( u_int number ): DO1( number, DT_DO, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������.
class siren : public DO1
    {
    public:
        siren( u_int number ): DO1( number, DT_HA, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������.
class lamp : public DO1
    {
    public:
        lamp( u_int number ): DO1( number, DT_HL, DST_NONE )
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
        counter( u_int number ): device(
            number, DT_FQT, DST_NONE, ADDITIONAL_PARAMS_COUNT ), 
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
                        
            P_MIN_FLOW = 0,
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

        /// @brief ��������� ���������� �� ��� ������.        
        device* get_device( int dev_type, u_int dev_number );

        /// @brief ��������� ������� �� ��� ������.
        i_DO_device* get_V( int number );

        /// @brief ��������� ������������ ������� �� ������.
        i_AO_device* get_VC( int number );

        /// @brief ��������� ��������� �� ������.
        i_DO_AO_device* get_M( int number );

        /// @brief ��������� ������ �� ������.
        i_DI_device* get_LS( int number );

        /// @brief ��������� ������� �� ������.
        i_DI_device* get_FS( int number );

        /// @brief ��������� ������� ��������� �� ������.
        i_DI_device* get_GS( int number );

        /// @brief ��������� ����������� ����� �� ������.
        i_AI_device* get_AI( int number );

        /// @brief ��������� ����������� ������ �� ������.
        i_AO_device* get_AO( int number );

        /// @brief ��������� �������� �� ������.
        i_counter* get_FQT( int number );

        /// @brief ��������� ����������� �� ������.
        i_AI_device* get_TE( int number );

        /// @brief ��������� �������� ������ �� ������.
        i_AI_device* get_LT( int number );

        /// @brief ��������� �������� ����� �� ������.
        i_DI_device* get_DI( int number );

        /// @brief ��������� ������ �� ������.
        i_DI_device* get_SB( int number );

        /// @brief ��������� ������������ ������ �� ������.
        i_DO_device* get_DO( int number );

        /// @brief ��������� �������� ������������.
        i_DO_device* get_HA( int number );

        /// @brief ��������� �������� ������������.
        i_DO_device* get_HL( int number );

        /// @brief ��������� ������� ������������ �� ������.
        i_AI_device* get_QT( int number );

        /// @brief ��������� ������������� ���������� ������.
        static device_manager* get_instance();

        /// @brief ���������� ������ ������� � �������.
        void print() const;

        const char* get_name() const
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

        /// @brief ��������� ������� ���������� �� ��� ������.        
        int get_device_n( device::DEVICE_TYPE dev_type, u_int dev_number );

        std::vector< device* > project_devices; ///< ��� ����������.

        /// @brief ������������ ��������� ������.
        static auto_smart_ptr < device_manager > instance;

    public:
        // @brief ��������� ����� ���������.
        //
        // ���������� �� Lua.
        wago_device* add_wago_device( int dev_type, int dev_sub_type, 
            u_int number, char * descr );  
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
i_DO_device* V( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������������ ������� �� ������.
///
/// @param number - ����� �������.
/// @return - ������ � �������� �������. ���� ��� ������ �������, ������������
/// �������� (@ref dev_stub).
i_AO_device* VC( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ��������� �� ������.
///
/// @param number - ����� ���������.
/// @return - ��������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_AO_device* M( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������ �� ������.
///
/// @param number - ����� ����������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* LS( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������� �� ������.
///
/// @param number - ����� ����������� �������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* FS( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ����� �� ������.
///
/// @param number - ����� ����������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* AI( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������ �� ������.
///
/// @param number - ����� ����������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AO_device* AO( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� �� ������.
///
/// @param number - ����� ��������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_counter* FQT( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� �� ������.
///
/// @param number - ����� �����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* TE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������ �� ������.
///
/// @param number - ����� �������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* LT( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ��������� �� ������.
///
/// @param number - ����� ������� ���������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* GS( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������ �� ������.
///
/// @param number - �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* HA( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������������ �� ������.
///
/// @param number - �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* HL( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ �� ������.
///
/// @param number - �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* SB( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ����� �� ������.
///
/// @param number - ����� �������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* DI( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ ���������� �� ������.
///
/// @param number - ����� ������ ����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* DO( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ������������ �� ������.
///
/// @param number - ����� ������� ������������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* QT( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������������ ����������.
///
/// @return - ����������� ����������.
dev_stub* STUB();
//-----------------------------------------------------------------------------
/// @brief ��������� ���������� �� ������ � ����.
///
/// @param type   - ��� ����������.
/// @param number - ����� ����������.
/// @return - ����������.
device* DEVICE( int type, int number );
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
