#ifndef _PID_H
#define _PID_H

#include "param_ex.h"
#include "util.h"
#include "sys.h"

class PID 
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

    //���� �� ��� ������ ���������� ���������, � �� ����������� �������� ��������.
    char is_down_to_inaccel_mode;  


    saved_params_float    *par;
    run_time_params_float *w_par;

    int state;

    int start_param_index;  
    enum PARAM 
        {
        PAR_Z,            //0 ��������� ��������.
        PAR_k,            //1 �������� k.
        PAR_Ti,           //2 �������� Ti.
        PAR_Td,           //3 �������� Td.
        PAR_dt,           //4 �������� �������
        PAR_dmax,         //5 �ax �������� ������� ��������.
        PAR_dmin,         //6 �in �������� ������� ��������.
        PAR_AccelTime,    //7 ����� ������ �� ����� �������������.
        PAR_IsManualMode, //8 ������ �����.
        PAR_UManual,      //9 �������� ������ �������� ��������� �������.
        PAR_Uk,           //10 ����� ���.

        PAR_k2,           //11 �������� k2.
        PAR_Ti2,          //12 �������� Ti2.
        PAR_Td2,          //13 �������� Td2. 
        }; 

    int start_work_params_index;
    enum WORK_PARAMS 
        {
        WPAR_Uk,          //1 ����� ���-����������.
        WPAR_Z,           //2 ��������� �������� ���.
        };

    enum ADDITIONAL_PARAM 
        {    
   
        };

    int   used_par_n;

    public:
        PID( saved_params_float* par, run_time_params_float *w_par,
            int start_param_index = 0, 
            int start_work_params_index = 0 );

        ~PID(); 
                
        void  on( char is_down_to_inaccel_mode = 0 );
        void  off();
        float eval( float current_value, int delta_sign = 1 );

        void  reset();
        void  reset( float new_uk_1 );
        void  set( float new_z );         //���������� ����� ������� ���.

        void init_param( int par_n, float val );
        void init_work_param( int par_n, float val );
        void save_param();

        void set_used_par ( int par_n ); //������������ kN, TiN, TdN.       
    };
#endif
