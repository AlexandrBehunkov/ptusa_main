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
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef TECH_DEFINITION_H
#define TECH_DEFINITION_H

#include <stdio.h>
#include <vector>
#include <locale>

#ifdef __BORLANDC__
extern "C" {
extern int snprintf(char *, size_t, const char *, /*args*/ ...);
    };
#endif // __BORLANDC__

#include "g_device.h"
#include "PAC_dev.h"

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
/// ��� �������������� ������� (�������, ������, ...) ������������ ��������� 
/// ������: ������� ������ �� ��������� � ������ ������ � ���.
///
class tech_object: public i_Lua_save_device
    {
    public:
        /// @param name                     - �������� ("��������" ...).
        /// @param number                   - �����.
        /// @param name_Lua                 - ��� � Lua.
        /// @param states_count             - ���������� �������.
        /// @param timers_count             - ���������� ��������.
        /// @param par_float_count          - ���������� ����������� ���������� ���� float.
        /// @param runtime_par_float_count  - ���������� ������� ���������� ���� float.
        /// @param par_uint_count           - ���������� ����������� ���������� ���� uint.
        /// @param runtime_par_uint_count   - ���������� ������� ���������� ���� uint.
        tech_object( const char* name, u_int number, const char* name_Lua,
            u_int states_count,
            u_int timers_count,
            u_int par_float_count, u_int runtime_par_float_count,
            u_int par_uint_count, u_int runtime_par_uint_count );

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

        int  evaluate();

        /// @brief �������� ����������� ��������� ������.
        ///
        /// ���� ������ ����� ���������� 1, �� ����� ����� �� ����������.
        ///
        /// @param mode - �����.
        ///
        /// @return 1 - ����� ������ ��������.
        /// @return 0 - ����� ����� ��������.
        int check_on_mode( u_int mode, char* reason );

        /// @brief ������������� ������.
        ///
        /// ��� ������������� ������ ���������� ������ �������� -
        /// ��������� ��������, ���������/���������� �������� � �.�.
        ///
        /// @param mode - �����.
        void init_mode( u_int mode );

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
            Print ( "\'%.40s\' - exec command command = %2d\n",
                name, cmd );
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

        // Lua implemented methods.
        int lua_exec_cmd( u_int cmd );

        int  lua_check_on_mode( u_int mode );
        void lua_init_mode( u_int mode );       
        int  lua_check_off_mode( u_int mode );
        int  lua_final_mode( u_int mode );
        int  lua_init_params();
        int  lua_init_runtime_params();
        // Lua implemented methods.

        
        // ������ � ��������.

        /// @brief �������� ������������� �������� ��������� �� ������ ��������
        /// ����� ����� ���������� ������.
        ///
        /// @return 0 - �� ���� ���������.
        /// @return 1 - ���� ���������.
        virtual int is_check_mode( int mode ) const;

        enum ERR_MSG_TYPES
            {
            ERR_CANT_ON,
            ERR_ON_WITH_ERRORS,
            ERR_OFF,
            ERR_OFF_AND_ON,
            ERR_DURING_WORK,
            ERR_SIMPLE,
            };

        struct  err_info
            {
            enum CONSTATS
                {
                MAX_STR_LENGTH = 150,
                };

            char          msg[ MAX_STR_LENGTH ];
            int           n;
            ERR_MSG_TYPES type;
            };

        int set_err_msg( const char *err_msg, int mode, int new_mode = 0, 
            ERR_MSG_TYPES type = ERR_CANT_ON );

        static const char* get_type( ERR_MSG_TYPES err_type );
        static int get_priority( ERR_MSG_TYPES err_type );

         std::vector< err_info* >& get_errors()
             {
             return errors;
             }
        // ������ � ��������.

        /// @brief ���������� ������ �������.
        void print() const
            {
            Print( "Object \'%.40s\' [%d]\n", name, number );
            modes_manager->print();
            }

        const char* get_name_in_Lua() const
            {
            static char tmp[ 100 ];
            snprintf( tmp, sizeof( tmp ), "%.40s",
                name_Lua );
            return tmp;
            }

        mode_manager* get_modes_manager()
            {
            return modes_manager;
            }

        int save_device( char *buff );

        int set_cmd( const char *prop, u_int idx, double val );

        int save_params_as_Lua_str( char* str );

        int set_param( int par_id, int index, double value );

    protected:
        u_int   number;         ///< ����� �������.
        u_int_4 cmd;            ///< �������� ������� �������.
        u_int   modes_count;    ///< ���������� �������.

        std::vector< u_int_4 >  state;  ///< ��������� �������.

        run_time_params_u_int_4 modes_time;    ///< ����� �������, ���.

        enum CONSTANTS
            {
            C_MAX_NAME_LENGTH = 30,
            };
        char name[ C_MAX_NAME_LENGTH ];        ///< ��� �������.
        char name_Lua[ C_MAX_NAME_LENGTH ];    ///< ��� ������� � Lua.


        smart_ptr< mode_manager > modes_manager; ///< ���� �������.

        enum PARAMS_ID
            {
            ID_PAR_FLOAT = 1,
            ID_RT_PAR_FLOAT,
            ID_PAR_UINT,
            ID_RT_PAR_UINT,            
            };

        // ������ � ��������.
        std::vector< err_info* > errors;
        // ������ � ��������.
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

        /// @brief ��������� ������� � �������� �������� �������.
        int get_object_with_active_mode( u_int mode, u_int start_idx,
            u_int end_idx );

        /// @brief ��������� ������� �� ������.
        tech_object* get_tech_objects( u_int idx );

        /// @brief ��������� ���������� ��������.
        u_int get_count() const;

        /// @brief ���������� �������� ���������������� ��������.
        void evaluate();

        /// @brief ������������� �������� �� ������ ������� �������� Lua.
        int init_objects();

        ~tech_object_manager();

        /// @brief ���������� ���������������� �������.
        void add_tech_object( tech_object* new_tech_object );

        int save_params_as_Lua_str( char* str );

#ifdef __BORLANDC__
#pragma option -w-inl
#endif // __BORLANDC__

        /// @brief ���������� ������ �������.
        void print()
            {
            Print( "Technological objects manager [%d]:\n",
                tech_objects.size() );

            for ( u_int i = 0; i < tech_objects.size(); i++ )
                {
                tech_objects[ i ]->print();
                Print( "\n" );

#ifdef KEY_CONFIRM
                Print( "Press any key to continue..." );
                Getch();
                Print( "\n" );
#endif // KEY_CONFIRM
                }
            }       

#ifdef __BORLANDC__
#pragma option -w.inl
#endif // __BORLANDC__

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
