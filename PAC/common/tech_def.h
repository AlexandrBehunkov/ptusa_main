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
        virtual int set_mode( u_int mode, int new_state );

        /// @brief ��������� ��������� ������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� �������.
        /// @return 0 - ����� �� �������.
        int get_mode( u_int mode );

        /// @brief �������� ����������� ��������� ������.
        ///
        /// ���� ������ ����� ���������� 1, �� ����� ����� �� ����������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� ������ ��������.
        /// @return 0 - ����� ����� ��������.
        virtual int check_on_mode( u_int mode );

        /// @brief ���������� �������.
        ///
        /// ����� ����� ����������� �����-���� �������� (����������/�����������
        /// ������ ������).
        ///
        virtual int exec_cmd( u_int cmd )
            {
#ifdef DEBUG
            Print ( "Exec command %s[ %2d ] command = %2d\n",
                com_dev->get_name(), number, cmd );
#endif
            return 0;
            }

        /// @brief ������������� ������.
        ///
        /// ��� ������������� ������ ���������� ������ �������� -
        /// ��������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        virtual void init_mode( u_int mode );

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
        virtual int check_off_mode( u_int mode );

        /// @brief ���������� ������.
        ///
        /// ��� ���������� ������ ���������� ������ �������� -
        /// ���������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        ///
        /// @return 0 - ��.
        virtual int final_mode( u_int mode );

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

        int get_number() const
            {
            return number;
            }

        u_int get_modes_count() const
            {
            return modes_count;
            }
        
        saved_params_float  par_float;      ///< ����������� ���������, ��� float.
        run_time_params_float rt_par_float; ///< ������� ���������, ��� float.
        saved_params_u_int_4 par_uint;      ///< ����������� ���������, ��� u_int.
        run_time_params_u_int_4 rt_par_uint;///< ������� ���������, ��� u_int.

    protected:
        smart_ptr< complex_device > com_dev; ///< ����� � ��������.

        u_int   number;         ///< ����� �������.
        u_int_4 cmd;            ///< �������� ������� �������.
        u_int   modes_count;    ///< ���������� �������.

        std::vector< u_int_4 >  state;  ///< ��������� �������.
        timer_manager           timers; ///< ������� �������.

        std::vector< u_int_4 >  mode_start_time;    ///< ����� ������ ������.
        run_time_params_u_int_4 mode_time;          ///< ����� �������, ���.


    };
//-----------------------------------------------------------------------------
class tech_object_manager
    {
    public:
        /// @brief ������������� ����������� ���������� ���� ���������������
        /// ��������.
        int init_params();

        /// @brief ������������� ������� ���������� ���� ���������������
        /// ��������.
        int init_runtime_params();

        /// @brief ��������� ������������� ���������� ������.
        static tech_object_manager* get_instance();

        /// @brief ��������� ������������� ���������� ������.
        static int set_instance( tech_object_manager* new_instance );

        int get_object_with_active_mode( u_int mode, u_int start_idx,
            u_int end_idx )
            {
            for ( u_int i = start_idx; 
                i <= end_idx && i < tech_objects.size(); i++ )
                {
                if ( tech_objects.at( i )->get_mode( mode ) ) return i;
                }

            return -1;
            }

        tech_object* get_tech_objects( u_int idx )
            {
            return tech_objects.at( idx );
            }

        u_int get_count() const
            {
            return tech_objects.size();
            }

        void evaluate()
            {
            for ( u_int i = 0; i < tech_objects.size(); i++ )
                {
                tech_objects.at( i )->evaluate();
                }
            }

        void add_tech_object( tech_object* new_tech_object )
            {
            tech_objects.push_back( new_tech_object );
            }

    private:
        static tech_object_manager* instance; ///< ������������ ��������� ������.

        std::vector< tech_object* > tech_objects; ///< ��������������� �������.
    };
//-----------------------------------------------------------------------------
#define G_TECH_OBJECT_MNGR tech_object_manager::get_instance()
#define G_TECH_OBJECTS tech_object_manager::get_instance()->get_tech_objects
//-----------------------------------------------------------------------------
#endif // TECH_PROCESS_H
