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

class PID: public i_Lua_save_device, i_cmd_device
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

    void acceleration( float accel_time );

    float start_value;

    int number; ///< ����� ���.
    
    char name[ 50 ];

    public:
        static const int PARAM_CNT = 14;

        enum PARAM  
            {        
            P_k = 1,               ///< �������� k.
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

            P_out_max,             ///< �ax �������� �������� ��������.
            P_out_min,             ///< �in �������� �������� ��������.
            }; 

        enum WORK_PARAM 
            {
            WP_Z = 1,  ///< ��������� ��������.
            WP_U,      ///< ����� ���.
            };


        /// @param n - �����.
        PID( int n );

        virtual ~PID(); 
                
        /// @brief ��������� ���.
        void  on( char is_down_to_inaccel_mode = 0 );

        /// @brief ���������� ���.
        void  off();
		
		/// @brief ����� ���
		void reset();

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

        int save_device( char *buff );

#ifdef RM_PAC
        int rm_save_device_state( char *buff )
            {
            return 0;
            }
#endif // RM_PAC

        /// @brief ���������� ������ ������� � �������.
        virtual const char* get_name_in_Lua() const
            {
            return name;
            }

		/// @brief ��������� ����������
		u_int_4 get_state();

		virtual int set_cmd( const char *prop, u_int idx, double val );

		virtual int set_cmd( const char *prop, u_int idx, char *val );

	};
#endif
