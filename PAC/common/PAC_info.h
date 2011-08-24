#ifndef PAC_INFO_H
#define PAC_INFO_H

#include "dtime.h"

#include "g_device.h"
#include "param_ex.h"

class PAC_info: public i_Lua_save_device
    {
    public:
        virtual ~PAC_info();

        void eval();

        enum PARAMETERS
            {
            P_PARAMS_COUNT  = 2,  ///< ���������� ����������.

            P_CTR_ERROR_TIME = 0, ///< ����� �������� ������ ��������, ����.                                    
            P_MIX_FLIP_PERIOD = 1,///< �������� �������� ����� ��������, ���. 
            };

        saved_params_u_int_4 *par;

        void reset_params();

        int save_device( char *buff );

        void print() const;

        /// @brief ��������� ������������� ���������� ������ ��� ������.
        ///
        /// @return - ��������� �� ������������ ������ ������ @ref PAC_info.
        static PAC_info* get_instance();

    private:   
        PAC_info();

        static auto_smart_ptr < PAC_info > instance;///< ��������� ������.

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
