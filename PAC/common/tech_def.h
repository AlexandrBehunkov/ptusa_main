/// @file tech_def.h
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

#ifndef TECH_DEFINITION_H
#define TECH_DEFINITION_H

#include <stdio.h>
#include <vector>

#include "sys.h"
#include "g_device.h"
#include "PAC_dev.h"
#include "PAC_err.h"

#include "tcp_cmctr.h"
#include "param_ex.h"

#include "mode_mngr.h"

#ifdef  __cplusplus
extern "C" {
#endif

#include    "lua.h"
#include    "lauxlib.h"
#include    "lualib.h"

#ifdef  __cplusplus
    };
#endif
#include    "tolua++.h"

//-----------------------------------------------------------------------------
/// @brief ������ � ��������������� ��������.
///
/// ������� ����� ��� ���������������� ������� (�����, ��������). ��������
/// �������� ������ ������ - ������ � �������� � �.�.
class tech_object: public i_Lua_save_device
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

        const char* get_object_name()
            {
            return object_name;
            }

        virtual ~tech_object();

        /// @brief ���������/���������� ������.
        ///
        /// @param mode      - �����.
        /// @param new_state - ����� ��������� ������.
        ///
        /// @return  0 - ����� ������� (��������).
        /// @return  1 - ����� ����� ��� ������� (��������).
        /// @return  3 - ��� ������ ������.
        int set_mode( u_int mode, int new_state );

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
        int check_on_mode( u_int mode );

        /// @brief ������������� ������.
        ///
        /// ��� ������������� ������ ���������� ������ �������� -
        /// ��������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        void init_mode( u_int mode );

        /// @brief ���������� ���������� �������.
        ///
        /// ��� �������� ������ ���������� ������ �������� -
        /// ���������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @return 0 - ��.
        int evaluate();

        /// @brief �������� ����������� ���������� ������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� ������ ���������.
        /// @return 0 - ����� ����� ���������.
        int check_off_mode( u_int mode );

        /// @brief ���������� ������.
        ///
        /// ��� ���������� ������ ���������� ������ �������� -
        /// ���������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        ///
        /// @return 0 - ��.
        int final_mode( u_int mode );

        /// @brief �������������� ����������� ��������� ��������� 0.
        ///
        /// ������ ������� �������� ����������� � ����������������� ������.
        int init_params();

        /// @brief �������������� ������� ��������� ��������� 0.
        int init_runtime_params();

        /// @brief ���������� �������.
        ///
        /// ����� ����� ����������� �����-���� �������� (����������/�����������
        /// ������ ������).
        ///
        int exec_cmd( u_int cmd )
            {
#ifdef DEBUG
            Print ( "Exec command %s[ %2d ] command = %2d\n",
                get_object_name(), number, cmd );
#endif
            return 0;
            }

        int get_number() const
            {
            return number;
            }

        u_int get_modes_count() const
            {
            return modes_count;
            }

        saved_params_float      par_float;      ///< ����������� ���������, ��� float.
        run_time_params_float   rt_par_float;   ///< ������� ���������, ��� float.
        saved_params_u_int_4    par_uint;       ///< ����������� ���������, ��� u_int.
        run_time_params_u_int_4 rt_par_uint;    ///< ������� ���������, ��� u_int.

        timer_manager           timers;         ///< ������� �������.

        //--Lua implemented methods.
        int lua_exec_cmd( u_int cmd );

        int  lua_check_on_mode( u_int mode );
        void lua_init_mode( u_int mode );
        int  lua_evaluate();
        int  lua_check_off_mode( u_int mode );
        int  lua_final_mode( u_int mode );
        int  lua_init_params();
        int  lua_init_runtime_params();
        //--Lua implemented methods.--!>

        /// @brief ���������� ������ �������.
        void print() const
            {
            Print( "Object \"%s\"\n", name );
            modes_manager->print();
            }

        mode_manager* get_modes_manager()
            {
            return modes_manager;
            }

        int save_device( char *buff );

        int set_cmd( const char *prop, u_int idx, double val )
            {
            if ( strcmp( prop, "CMD" ) == 0 )
                {
                int mode = ( int ) val;
                if ( mode < 2000 )
                    {
                    set_mode( mode - 1000, 1 );
                    }
                else
                    {
                    set_mode( mode - 2000, 0 );
                    }
                cmd = mode;
                }

            if ( strcmp( prop, "PAR_F" ) == 0 )
                {
                par_float[ idx ] = ( float ) val;
                }
            
            return 0;
            }

    protected:
        u_int   number;         ///< ����� �������.
        u_int_4 cmd;            ///< �������� ������� �������.
        u_int   modes_count;    ///< ���������� �������.

        std::vector< u_int_4 >  state;  ///< ��������� �������.

        run_time_params_u_int_4 mode_time;          ///< ����� �������, ���.

        enum CONSTANTS
            {
            C_MAX_NAME_LENGTH = 30,
            };
        char name[ C_MAX_NAME_LENGTH ];        ///< ��� ������� + ����� �������.
        char object_name[ C_MAX_NAME_LENGTH ]; ///< ��� �������.


        smart_ptr< mode_manager > modes_manager; ///< ���� �������.
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

        /// @brief ��������� ������� � �������� �������� �������.
        int get_object_with_active_mode( u_int mode, u_int start_idx,
            u_int end_idx );

        /// @brief ��������� ������� �� ������.
        tech_object* get_tech_objects( u_int idx );

        /// @brief ��������� ���������� ��������.
        u_int get_count() const;

        /// @brief ���������� �������� ���������������� ��������.
        void evaluate();

        int init_objects();

        ~tech_object_manager();

        /// @brief ���������� ���������������� �������.
        void add_tech_object( tech_object* new_tech_object );

        /// @brief ���������� ������ �������.
        void print()
            {
            Print( "Technological objects manager\n" );
            for ( u_int i = 0; i < tech_objects.size(); i++ )
                {
                tech_objects[ i ]->print();

#ifdef KEY_CONFIRM
                Print( "Press any key to continue..." );
                Getch();
                Print( "\n" );
#endif // KEY_CONFIRM
                }
            }

    private:
        /// ������������ ��������� ������.
        static auto_smart_ptr < tech_object_manager > instance;

        std::vector< tech_object* > tech_objects; ///< ��������������� �������.
    };
//-----------------------------------------------------------------------------
tech_object_manager* G_TECH_OBJECT_MNGR();
//-----------------------------------------------------------------------------
tech_object* G_TECH_OBJECTS( u_int idx );
//-----------------------------------------------------------------------------
#endif // TECH_DEFINITION_H
