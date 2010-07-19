/// @file tech_proc.h
/// @brief ����������� ������� ���������������� ��������� ��������, �����������
/// ��������������� ������� - ���� (��������������� �������),
/// �������� (������������ ��������) � �.�.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.
///
/// @par ������� ������:
/// @$Rev: 96 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-07-09 15:57:50#$.

#ifndef TECH_PROCESS_H
#define TECH_PROCESS_H

#include <stdio.h>
#include <vector>

#include "sys.h"
#include "g_device.h"
#include "PAC_dev.h"
#include "PAC_err.h"

#include "tcp_cmctr.h"
#include "param_ex.h"

//-----------------------------------------------------------------------------
/// @brief ������ � ��������������� ��������.
///
/// ������� ����� ��� ���������������� ������� (�����, ��������). ��������
/// �������� ������ ������ - ������ � �������� � �.�.
class tech_object
    {
    public:
        /// @param name                     - �������� (tank, comb).
        /// @param number                   - �����.
        /// @param states_count             - ���������� �������.
        /// @param timers_count             - ���������� ��������.
        /// @param par_float_count          - ���������� ����������� ���������� ���� float.
        /// @param runtime_par_float_count  - ���������� ������� ���������� ���� float.
        /// @param par_uint_count           - ���������� ����������� ���������� ���� uint.
        /// @param runtime_par_uint_count   - ���������� ������� ���������� ���� uint.
        tech_object( const char* name = "tech_object", u_int number = 0,
            u_int states_count = 1,
            u_int timers_count = 3,
            u_int par_float_count = 5, u_int runtime_par_float_count = 5,
            u_int par_uint_count = 5, u_int runtime_par_uint_count = 5 );
       
        virtual ~tech_object();

        /// @brief ���������/���������� ������.
        ///
        /// @param mode      - �����.
        /// @param new_state - ����� ��������� ������.
        ///
        /// @return  0 - ��.
        /// @return >0 - ������.
        int set_mode( u_int mode, int new_state );

        /// @brief ��������� ��������� ������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� �������.
        /// @return 0 - ����� �� �������.
        inline int get_mode( int mode );

        /// @brief �������� ����������� ��������� ������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� ������ ��������.
        /// @return 0 - ����� ����� ��������.
        virtual int can_init_mode( int mode );

        /// @brief ������������� ������.
        ///
        /// ��� ������������� ������ ���������� ������ �������� -
        /// ��������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        ///
        /// @return 0 - ��.
        virtual int init_mode( int mode );

        /// @brief ���������� ���������� �������.
        ///
        /// ��� �������� ������ ���������� ������ �������� -
        /// ���������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @return 0 - ��.
        virtual int evaluate();

        /// @brief �������� ����������� ���������� ������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� ������ ���������.
        /// @return 0 - ����� ����� ���������.
        virtual int can_final_mode( int mode );

        /// @brief ���������� ������.
        ///
        /// ��� ���������� ������ ���������� ������ �������� -
        /// ���������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        ///
        /// @return 0 - ��.
        virtual int final_mode( int mode );

        /// @brief �������������� ����������� ��������� ��������� 0.
        ///
        /// ������ ������� �������� ����������� � ����������������� ������.
        virtual int init_params();

        /// @brief �������������� ������� ��������� ��������� 0.
        virtual int init_runtime_params();

        complex_device* get_complex_dev()
            {
            return com_dev;
            }

    protected:
        smart_ptr< complex_device > com_dev; ///< ����� � ��������.

        u_int   number;         ///< ����� �������.
        u_int_4 cmd;            ///< �������� ������� �������.
        u_int   states_count;   ///< ���������� �������.

        std::vector< u_int_4 >  state;  ///< ��������� �������.
        timer_manager           timers; ///< ������� �������.

        std::vector< u_int_4 >  mode_start_time;        ///< ����� ������ ������.
        smart_ptr< run_time_params_u_int_4 > mode_time; ///< ����� �������, ���.

        /// @brief ����������� ���������, ��� float.
        smart_ptr< saved_params_float >      par_float;

        /// @brief ������� ���������, ��� float.
        smart_ptr< run_time_params_float >   rt_par_float;

        /// @brief ����������� ���������, ��� unsigned int.
        smart_ptr< saved_params_u_int_4 >    par_uint;

        /// @brief ������� ���������, ��� unsigned int.
        smart_ptr< run_time_params_u_int_4 > rt_par_uint;
    };
//-----------------------------------------------------------------------------
class tech_object_manager
    {
    public:
        /// @brief ������������� ����������� ���������� ���� ��������������� ��������.
        int init_params();

        /// @brief ������������� ������� ���������� ���� ��������������� ��������.
        int init_runtime_params();

        /// @brief ��������� ������������� ���������� ������.
        static tech_object_manager* get_instance();

        /// @brief ��������� ������������� ���������� ������.
        static int set_instance( tech_object_manager* new_instance );

    private:
        static tech_object_manager* instance; ///< ������������ ��������� ������.

        std::vector< tech_object > tech_objects; ///< ��������������� �������.
    };
//-----------------------------------------------------------------------------
#endif // TECH_PROCESS_H
