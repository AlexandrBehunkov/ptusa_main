/// @file PID.h
/// @brief �������� �������� ������ ���-�����������.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef _PID_H
#define _PID_H

#include "dtime.h"

#include "param_ex.h"
#include "PAC_dev.h"
//#include "sys.h"

class PID: public i_Lua_save_device
    { 
    enum STATES 
        {
        STATE_OFF,
        STATE_ON,  
        };

    float uk_1;
    float ek_1;
    float ek_2;
    float q0;
    float q1;
    float q2;
    float Uk;
    float dUk;
    unsigned long start_time;
    unsigned long last_time;

    char prev_manual_mode;

    /// @brief ���� �� ��� ������ ���������� ���������, � �� �����������
    /// �������� ��������.
    char is_down_to_inaccel_mode;  

    saved_params_float    *par;
    run_time_params_float *w_par;

    u_int_4 state;

    int   used_par_n;

    void reset();
    void acceleration( float accel_time );

    float start_value;

    int number; ///< ����� ���.
    
    char name[ 50 ];

    public:
        enum PARAM  
            {        
            P_k = 0,               ///< �������� k.
            P_Ti,                  ///< �������� Ti.
            P_Td,                  ///< �������� Td.
            P_dt,                  ///< �������� �������
            P_max,                 ///< �ax �������� ������� ��������.
            P_min,                 ///< �in �������� ������� ��������.
            P_acceleration_time,   ///< ����� ������ �� ����� �������������.
            P_is_manual_mode,      ///< ������ �����.
            P_U_manual,            ///< �������� ������ �������� ��������� �������.        

            P_k2,                  ///< �������� k2.
            P_Ti2,                 ///< �������� Ti2.
            P_Td2,                 ///< �������� Td2. 
            }; 

        enum WORK_PARAM 
            {
            WP_Z = 0,  ///< ���������f�� ��������.
            WP_U,      ///< ����� ���.
            };


        /// @param n - �����.
        PID( int n );

        virtual ~PID(); 
                
        /// @brief ��������� ���.
        void  on( char is_down_to_inaccel_mode = 0 );

        /// @brief ���������� ���.
        void  off();

        /// @brief ������ ���.
        float eval( float current_value, int delta_sign = 1 );

        /// @brief ��������� ������ ������� ���.
        void set( float new_z );    

        /// @brief ��������� ������� ���.
        float get_assignment();

        /// @brief ������������� ��������� ���.
        void init_param( PARAM par_n, float val );

        /// @brief ������������� �������� ��������� ���.
        void init_work_param( WORK_PARAM par_n, float val );

        /// @brief ���������� ���������� ���.
        void save_param();

        /// @brief ���������� ����� � ������� ���.
        void print() const;

        /// @brief ������������� kN, TiN, TdN.     
        void set_used_par ( int par_n ); 


        int save_device( char *buff )
            {
            int answer_size = 0;

            sprintf( buff, "t.%s = \n\t{\n", name );
            answer_size += strlen( buff );

            //���������.
            answer_size += par->save_device( buff + answer_size, "\t" );
            answer_size += w_par->save_device( buff + answer_size, "\t" );

            sprintf( buff + answer_size, "\t}\n" );
            answer_size += strlen( buff + answer_size );

            return answer_size;
            }

        /// @brief ���������� ������ ������� � �������.
        virtual const char* get_name() const
            {
            return name;
            }
    };
#endif
