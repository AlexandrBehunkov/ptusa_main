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

//#include "sys.h"
#include "wago.h"
#include "g_device.h"

//class saved_params_u_int_4;
#include "param_ex.h"

#define OFF     0
#define ON      1

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

        virtual ~i_counter()
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� �����.
///
/// �������� �����, ���������� ������� � �.�. �������� ��������� �����
/// ���������.
class i_DI_device
    {
    public:
        i_DI_device();

        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @return - ������� ��������� ���������� � ���� ������ �����.
        virtual int get_state_now() = 0;

        /// @brief ��������� ���������������� ��������� ����������.
        ///
        /// ���������� ������ ���� ���������, ���� ���������� ���� ��������� �
        /// � ������ ��������� ������ ��������� ��������� ������� (�������
        /// ��������� - ������ @ref set_change_time).
        ///
        /// @return - ��������� ���������� � ���� ������ �����.
        virtual int get_state();

        /// @brief ��������� ������� �������� ��������� ���������.
        ///
        /// @param time - ����� �������� ��������� ���������.
        void set_change_time( u_int time );

        /// @brief ��������� ���������.
        ///
        /// ������ ����� ������������ ��� ������� ��������� ���������� �����
        /// ��� ���������.
        ///
        /// @param new_state - ����� ���������.
        void set_state( int new_state );

        /// @brief �������� ��������� ���������.
        ///
        /// ��� ������ - �������� �� ��� ���.
        ///
        /// @return - 1 - ���������� �������.
        /// @return - 0 - ���������� ���������.
        virtual bool is_active();

    protected:
        u_int last_check_time;///< ����� ��������� �������� ���������.
        int   state;          ///< ���������� ���������, ��� ���������� ��������.
        u_int dt;             ///< �������� ������������ ���������, ����.
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� ������.
///
/// ������, ������� � �.�. �������� ��������� ����� ���������.
class i_DO_device: public i_DI_device
    {
    public:
        /// @brief ��������� ����������.
        ///
        /// ��������� ���������� � �������� ���������. ��� ������� ��� ��������
        /// ��� �������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void on() = 0;

        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;

        /// @brief ��������� ������ ��������� ����������.
        ///
        /// @param new_state - ����� ��������� �������.
        virtual void set_state( int new_state ) = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� �� ������ ����������� �����.
///
/// �����������, ������ � �.�. �������� ��������� ����� ���������.
class i_AI_device
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
        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;

        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @param new_value - ����� ��������� ����������.
        virtual void set_value( float new_value ) = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ����� �������������� �������� ����������, ������� ������������ � 
/// �������.
class device : public i_AO_device,    
    public i_DO_device, public i_cmd_device
    {
    public:
        int set_cmd( const char *prop, u_int idx, double val ) 
            {
            if ( strcmp( prop, "st" ) == 0 )
                {
                set_state( ( int ) val );
                }

            if ( strcmp( prop, "v" ) == 0 )
                {
                set_value( ( float ) val );
                }
            
            return 0;
            }

        int set_cmd( const char *prop, u_int idx, char *val )
            {
            return 0;
            }

        virtual int save_device( char *buff, const char *prefix )
            {
            sprintf( buff, "%s[%d]={st=%d, v=%.2f},\n", 
                prefix, get_n(),  get_state(), get_value() );

            return strlen( buff );
            }

        //-������.
        saved_params_u_int_4 err_par;

        enum DEV_ERROR_PARAMS  ///< ��������� ������, ���������� ������ �����.
            {
            DE_IS_ENABLE = 1,  ///< ���������� ������� �� ����� ��������������.    
            DE_IS_INHIBIT = 2, ///< ���������� ������� �� ����� ������.    
            DE_IS_SUPPRESS = 4,///< ���������� ������� ���������.
            };
        //-������.!->

        enum CONSTANTS
            {
            C_DEVICE_TYPE_CNT = 14,     ///< ���������� ����� ���������.
            };

        static const char DEV_NAMES[][ 5 ];
        static const char DEV_TYPES[];

        /// ���� ���������.
        enum DEVICE_TYPE
            {
            DT_NONE = -1,      ///< ��� �� ���������.

            DT_V = 0,   ///< ������. 
            DT_N,       ///< �����.
            DT_M,       ///< �������.
            DT_LS,      ///< ������� (����/���).
            DT_TE,      ///< �����������.
            DT_FE,      ///< ������ (��������).
            DT_FS,      ///< ������ (����/���).
            DT_CTR,     ///< �������.
            DT_AO,      ///< ���������� �����.
            DT_LE,      ///< ������� (��������).
            DT_FB,      ///< �������� �����.
            DT_UPR,     ///< ����� ����������.
            DT_QE,      ///< ������������.
            DT_AI,      ///< ���������� ����.
            };

        /// ������� ���������.
        enum DEVICE_SUB_TYPE
            {
            DST_NONE = -1,      ///< ������ �� ���������.

            //DT_V = 0, 
            DST_V_DO_1 = 1,     ///< ������ � ����� ������� ����������.
            DST_V_DO_2,         ///< ������ � ����� �������� ����������.
            DST_V_DO_1_DI_1,    ///< ������ � ����� ������� ���������� � ����� �������� ������.
            DST_V_DO_1_DI_2,    ///< ������ � ����� ������� ���������� � ����� ��������� �������.
            DST_V_DO_2_DI_2,    ///< ������ � ����� �������� ���������� � ����� ��������� �������.
            DST_V_MIXPROOF,     ///< ������ ��������.

            DST_V_1DO_3DI,      ///< ������ � ����� ������� ���������� � ����� ��������� �������.
            DST_V_1DO_2DI_S,    ///< ������ � ����� ������� ���������� � ����� ��������� ������� �� ���� �� ���������.
            DST_V_AS_MIXPROOF,       ///< ������ � ����� �������� ���������� � ����� ��������� ������� � AS ����������� (��������).
            };

        device( int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): err_par( 1 ),
            number( number ),
            type( type ),
            sub_type( sub_type )            
            {
            }

        virtual ~device()
            {           
            }

        char *get_name() const
            {
            return "";
            }

        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;

        /// @brief ����� ������� � �������.
        ///
        /// ��� ������������� � ���������� �����.
        virtual void print() const;

        /// @brief ��������� ������ ����������.
        ///
        /// @return -  ����� ����������.
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

    protected:
        u_int_4 number;             ///< ����� ����������.

        DEVICE_TYPE     type;       ///< ��� ����������.
        DEVICE_SUB_TYPE sub_type;   ///< ������ ����������.
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
        dev_stub(): device( 0, DT_NONE, DST_NONE )
            {
            }

        u_int_4 get_n() const;                
        void    print() const;                

        float   get_value();
        void    set_value( float new_value );

        void    on();                
        void    off();                
        void    set_state( int new_state );
        int     get_state_now();

        void    pause();
        void    start();
        void    reset();        
        u_int   get_quantity();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ����� ��� ��������� ���������� ���������.
class digital_device : public device,  
    public wago_device
    {
    public:
        digital_device( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): device( number, type, sub_type )
#ifdef DEBUG_NO_WAGO_MODULES
            , state( 0 )
#endif // DEBUG_NO_WAGO_MODULES
            {
            }

        virtual ~digital_device()
            {
            }

        int parse_cmd( char* cmd ) 
            {
            int val = 0;
            return 4;
            }

        float   get_value();
        void    set_value( float new_value );
        void    set_state( int new_state );  
        void    print() const;

        int save_device( char *buff, const char *prefix )
            {	
            sprintf( buff, "%s[%d]={st=%d},\n", 
                prefix, get_n(),  get_state() );

            return strlen( buff );
            }

#ifdef DEBUG_NO_WAGO_MODULES
        /// @brief ��������� ����������� ��������� �������.        
        ///        
        /// @return - ���������� ��������� �������.
        int  get_state_now();

        void on();
        void off();
#endif // DEBUG_NO_WAGO_MODULES

        int get_state();

    protected:
        enum CONSTANTS
            {
            C_SWITCH_TIME = 3, ///< ����� ������������ ����������, ���.
            };

    private:
#ifdef DEBUG_NO_WAGO_MODULES
        char state;  ///< ��������� ����������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ����� ��� ��������� ���������� ���������.
class analog_device : public device,   
    public wago_device
    {
    public:
        analog_device( int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): device( number, type, sub_type )
#ifdef DEBUG_NO_WAGO_MODULES
            ,value( 0 )
#endif  // DEBUG_NO_WAGO_MODULES
            {
            }

        void  set_state( int new_state );
        int   get_state_now();
        
        void  print() const;
        void  on();        
        void  off();

        int save_device( char *buff, const char *prefix )
            {
            sprintf( buff, "%s[%d]={v=%.2f},\n", 
                prefix, get_n(), get_value() );

            return strlen( buff );
            }

#ifdef DEBUG_NO_WAGO_MODULES
        float get_value();
        void set_value( float new_value );

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
class DO_1 : public digital_device
    {
    public:
        DO_1( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

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
class DO_2 : public digital_device
    {
    public:
        DO_2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();        

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0, ///< ������ ������ ����������� ������ �1.
            DO_INDEX_2,     ///< ������ ������ ����������� ������ �2.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ���������� ������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_1 : public digital_device
    {
    public:
        DO_1_DI_1( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.
            DI_INDEX = 0,           ///< ������ ������ ����������� �����.
            };

        u_long start_switch_time;  ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_2 : public digital_device
    {
    public:
        DO_1_DI_2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.

            DI_INDEX_1 = 0,         ///< ������ �1 ������ ����������� �����.
            DI_INDEX_2,             ///< ������ �2 ������ ����������� �����.
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� �������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_2_DI_2 : public digital_device
    {
    public:
        DO_2_DI_2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0,         ///< ������ ������ ����������� ������ �1.
            DO_INDEX_2,             ///< ������ ������ ����������� ������ �2.

            DI_INDEX_1 = 0,         ///< ������ �1 ������ ����������� �����.
            DI_INDEX_2,             ///< ������ �2 ������ ����������� �����.
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ mixproof.
class valve_mix_proof : public digital_device
    {
    public:
        valve_mix_proof( u_int number ): digital_device( number, DT_V, DST_V_MIXPROOF )
            {
            }

        enum STATES
            {
            ST_CLOSE = 0,   ///< ������.
            ST_OPEN,        ///< ������.
            ST_UPPER_SEAT,  ///< ������� ������� �����.
            ST_LOW_SEAT,    ///< ������� ������ �����.
            };

        void open_upper_seat();
        void open_low_seat();

#ifndef DEBUG_NO_WAGO_MODULES
        int  get_state_now();
        void on();
        void off();
        void set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            DO_INDEX_U,     ///< ������ ������ ����������� ������ �������� �����.
            DO_INDEX_L,     ///< ������ ������ ����������� ������ ������� �����.

            DI_INDEX_U = 0, ///< ������ ������ ����������� ����� �������� �����.
            DI_INDEX_L,     ///< ������ ������ ����������� ����� ������� �����.
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ AS-mixproof.
class valve_AS_mix_proof : public digital_device
    {
    public:
        valve_AS_mix_proof( u_int number );

        enum STATES
            {
            ST_CLOSE = 0,   ///< ������.
            ST_OPEN,        ///< ������.
            ST_UPPER_SEAT,  ///< ������� ������� �����.
            ST_LOW_SEAT,    ///< ������� ������ �����.
            };

        void open_upper_seat();
        void open_low_seat();

#ifndef DEBUG_NO_WAGO_MODULES
        int  get_state_now();
        void on();
        void off();
        void set_state( int new_state );

    private:
        enum CONSTANTS
            {
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �����������, ������ (��������)...
class AI_1 : public analog_device
    {
    public:
        AI_1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): analog_device( number, type, sub_type )
            {
            }
#ifndef DEBUG_NO_WAGO_MODULES
    public:
        float get_value();
        void   set_value( float new_value );

        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ������ ����������.
        virtual float get_min_val() = 0;

    protected:
        enum CONSTANTS
            {
            AI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief �����������.
class temperature_e : public AI_1
    {
    public:
        temperature_e( u_int number ): AI_1( number, DT_TE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������� �������.
class level_e : public AI_1
    {
    public:
        level_e( u_int number ): AI_1( number, DT_LE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������� �����.
class flow_e : public AI_1
    {
    public:
        flow_e( u_int number ): AI_1( number, DT_FE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_MIN_PAR_NUMBER = 0,   ///< ������ ��������� ������������ ��������.
            C_MAX_PAR_NUMBER,       ///< ������ ��������� ������������� ��������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������������.
class concentration_e : public AI_1
    {
    public:
        concentration_e( u_int number ): AI_1( number, DT_QE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_MIN_PAR_NUMBER = 0,   ///< ������ ��������� ������������ ��������.
            C_MAX_PAR_NUMBER,       ///< ������ ��������� ������������� ��������.
            };                      
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ����������� ������ 4-20 ��.
class analog_input_4_20 : public AI_1
    {
    public:
        analog_input_4_20( u_int number ): AI_1( number, DT_AI, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ����������� ������...
class AO_1 : public analog_device
    {
    public:
        AO_1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): analog_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ����� ����������.
        virtual float get_min_val() = 0;

        float get_value();
        void  set_value( float new_value );

    private:
        enum CONSTANTS
            {
            AO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� ����������� ������...
class AO_0_100 : public AO_1
    {
    public:
        AO_0_100( u_int number ) : AO_1( number, DT_AO, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_AO_MIN_VALUE = 0,     ///< ������ ��������� ������������ ��������.
            C_AO_MAX_VALUE = 100,   ///< ������ ��������� ������������� ��������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �������� �����, ������ (����/���)...
class DI_1 : public digital_device
    {
    public:
        DI_1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type, u_int dt = 0 ):
        digital_device( number, type, sub_type )
            {
            set_change_time( dt );
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ����������.
class valve_DO_1 : public DO_1
    {
    public:
        valve_DO_1( u_int number ) : DO_1( number, DT_V, DST_V_DO_1 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ����������.
class valve_DO_2 : public DO_2
    {
    public:
        valve_DO_2( u_int number ): DO_2( number, DT_V, DST_V_DO_2 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� �������� ������.
class valve_DO_1_DI_1 : public DO_1_DI_1
    {
    public:
        valve_DO_1_DI_1( u_int number ): DO_1_DI_1( number, DT_V, DST_V_DO_1_DI_1 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� ��������� �������.
class valve_DO_1_DI_2 : public DO_1_DI_2
    {
    public:
        valve_DO_1_DI_2( u_int number ): DO_1_DI_2( number, DT_V, DST_V_DO_1_DI_2 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ���������� � ����� ��������� �������.
class valve_DO_2_DI_2 : public DO_2_DI_2
    {
    public:
        valve_DO_2_DI_2( u_int number ): DO_2_DI_2( number, DT_V, DST_V_DO_2_DI_2 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief �����.
class pump : public DO_1_DI_1
    {
    public:
        pump( u_int number ): DO_1_DI_1( number, DT_N, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief �������.
class mixer : public DO_1_DI_1
    {
    public:
        mixer( u_int number ): DO_1_DI_1( number, DT_M, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� ������.
class level_s : public DI_1
    {
    public:
        level_s( u_int number, u_int dt = 1000 );

        bool is_active();

    private:
        enum CONSTANTS
            {
            C_ACTIVE_STATE = 0
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� �������.
class flow_s : public DI_1
    {
    public:
        flow_s( u_int number ): DI_1( number, DT_FS, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ �������� �����.
class feedback : public DI_1
    {
    public:
        feedback( u_int number ): DI_1( number, DT_FB, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ ����������
class control_s : public DO_1
    {
    public:
        control_s( u_int number ): DO_1( number, DT_UPR, DST_NONE )
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
        counter( u_int number ): device( number, DT_CTR, DST_NONE ), 
            value( 0 ),
            last_read_value( 0 )
            {            
            }

        virtual ~counter()
            {            
            }

        float get_value();
        void  set_value( float new_value );
        int   get_state_now();
        void  on();
        void  off();
        void  set_state( int new_state );
        void  print() const;

        void  pause();
        void  start();
        void  reset();
        u_int get_quantity();

    private:
        enum STATES
            {
            S_STOP,
            S_WORK,
            S_PAUSE,
            };

        enum CONSTANTS
            {
            AI_INDEX = 0,       ///< ������ ������ ����������� �����.

            MAX_VAL = 65535,    ///< ������������ �������� ��������.
            };

        u_int value;
        u_int last_read_value;

        STATES state;
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������.
///
/// �������� ���������� ��� ���� ����������� �������.
class device_manager: public i_Lua_save_device
    {
    public:
        device_manager();

        ~device_manager();

        /// @brief ��������� ���������� �� ��� ������.        
        device* get_device( device::DEVICE_TYPE dev_type, u_int dev_number );

        /// @brief ��������� ������� �� ��� ������.
        i_DO_device* get_V( int number );

        /// @brief ��������� ������ �� ������.
        i_DO_device* get_N( int number );

        /// @brief ��������� ������� �� ������.
        i_DO_device* get_M( int number );

        /// @brief ��������� ������ �� ������.
        i_DI_device* get_LS( int number );

        /// @brief ��������� ������� �� ������.
        i_DI_device* get_FS( int number );

        /// @brief ��������� ����������� ����� �� ������.
        i_AI_device* get_AI( int number );

        /// @brief ��������� ����������� ������ �� ������.
        i_AO_device* get_AO( int number );

        /// @brief ��������� �������� �� ������.
        i_counter* get_CTR( int number );

        /// @brief ��������� ����������� �� ������.
        i_AI_device* get_TE( int number );

        /// @brief ��������� �������� ������� �� ������.
        i_AI_device* get_FE( int number );

        /// @brief ��������� �������� ������ �� ������.
        i_AI_device* get_LE( int number );

        /// @brief ��������� �������� ����� �� ������.
        i_DI_device* get_FB( int number );

        /// @brief ��������� ������������ ������ �� ������.
        i_DO_device* get_UPR( int number );

        /// @brief ��������� ������� ������������ �� ������.
        i_AI_device* get_QE( int number );

        /// @brief ��������� ������������� ���������� ������.
        static device_manager* get_instance();

        /// @brief ���������� ������ ������� � �������.
        void print() const;

        /// @brief ��������� ���������� ����������.
        dev_stub* get_stub()
            {
            return &stub;
            }

        int save_device( char *buff )
            {
            sprintf( buff, "t=t or {}\n\r" );
            int answer_size = strlen( buff );

            for ( int i = 0; i < device::C_DEVICE_TYPE_CNT; i++)
                {
                sprintf( buff + answer_size, "t.%s=\n\t{\n", device::DEV_NAMES[ i ] );
                answer_size += strlen( buff + answer_size );

                int l = dev_types_ranges[ i ].start_pos;
                int u = dev_types_ranges[ i ].end_pos;

                if ( -1 != l ) // ���� ����������.
                    {
                    for ( int j = l; j <= u; j++ )
                        {
                        answer_size += project_devices[ j ]->save_device( buff + answer_size, "\t");
                        }
                    }

                sprintf( buff + answer_size, "\t}\n" );
                answer_size += strlen( buff + answer_size );
                }

            return answer_size;
            }

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
        wago_device* add_device( int dev_type, int dev_sub_type, 
            u_int number, char * descr );

        void complete_init()
            {
            }
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
        timer& operator[] ( unsigned int index );

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
/// @brief ��������� ������ �� ������.
///
/// @param number - ����� ������.
/// @return - ����� � �������� �������. ���� ��� ������ ������, ������������
/// �������� (@ref dev_stub).
i_DO_device* N( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� �� ������.
///
/// @param number - ����� �������.
/// @return - ������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* M( int number );
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
i_counter* CTR( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� �� ������.
///
/// @param number - ����� �����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* TE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������� �� ������.
///
/// @param number - ����� �������� �������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* FE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������ �� ������.
///
/// @param number - ����� �������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* LE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ����� �� ������.
///
/// @param number - ����� �������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* FB( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ ���������� �� ������.
///
/// @param number - ����� ������ ����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* UPR( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ������������ �� ������.
///
/// @param number - ����� ������� ������������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* QE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ �� ������.
///
/// @return - ����������� ����������.
dev_stub* STUB();
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
