#ifndef PAC_INFO_H
#define PAC_INFO_H

#include "g_device.h"
#include "PAC_dev.h"
#include "param_ex.h"

class PAC_info: public i_Lua_save_device
    {
    public:
        PAC_info();

        ~PAC_info();

        void eval();

        enum PARAMETERS
            {
            P_PARAMS_COUNT  = 1,  // ���������� ����������.

            P_CTR_ERROR_TIME = 0, // ����� �������� ������ ��������, ����.
            };

        saved_params_u_int_4 *par;

        void reset_params();

        int save_device( char *buff );

		void print() const
			{
			Print( "PAC_info\n" );
			}

    private:    
        static const u_int_4 MSEC_IN_DAY; 
        enum CONSTANTS
            {
            C_MAX_STR_LENGTH = 50,
            };

        u_int_4 up_days;
        u_int_4 up_hours;
        u_int_4 up_mins;
        u_int_4 up_secs;

        u_int_4 up_msec;
        u_int_4 last_check_time;

        u_int_4 reset_type;

        char up_time_str[ C_MAX_STR_LENGTH ];
    };
#endif // PAC_INFO_H
