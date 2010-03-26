/// @file param_ex.h
/// @brief �������� �������� �������, ������� ��������� ������ � �����������.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c USE_SIMPLE_DEV_ERRORS   - ���������� � ������� ������ ���������.@n
/// @c USE_NO_TANK_COMB_DEVICE - ���������� ��� �������� ���� � ��������.
/// 
/// @par ������� ������:
/// @$Rev: $.\n
/// @$Author: $.\n
/// @$Date::                    #$.

#ifndef PARAMS_EX_H
#define PARAMS_EX_H

#include <string.h>

#include "g_device.h"
#include "plc_dev.h"

#ifdef USE_SIMPLE_DEV_ERRORS
#include "errors.h"

extern dev_errors_manager *g_dev_errors_manager; 
#endif // USE_SIMPLE_DEV_ERRORS

#ifndef USE_NO_TANK_COMB_DEVICE
#include "priem.h"

extern TTank            **g_tanks;
extern TMyComb          **g_combs;
#endif //USE_NO_TANK_COMB_DEVICE
//-----------------------------------------------------------------------------
#include "sys.h"
#include "sys_7186.h"
//-----------------------------------------------------------------------------
/// @brief ������ � �����������. 
/// 
/// ������ ��� �������� �������� ��� ������ � �����������. ��������� 
/// ������������ ������ ����� �������� ����������� �����.
class params_manager
    {
    public:
        enum CONSTANTS
            {
            C_TOTAL_PARAMS_SIZE = 700 * 4, ///< ����� ������ ������ ����������.
            };

        /// @brief ���������� ������������ ��������� ������ ��� ������ � 
        /// �����������.
        ///
        /// @return - ��������� �� ������������ ������ ������ @ref
        /// params_manager.
        static params_manager* get_instance();

        /// @brief ������������� �������� ����������.
        ///
        /// ���������� ���������� �� EEPROM � ������ ����������, �������� 
        /// ����������� �����. ����������� ����� ������� �� 
        /// ��������� @ref C_TOTAL_PARAMS_SIZE.
        ///
        /// @param project_id - ����� �������. ��� �������� ���������� � 
        /// ���������� ����������� ���������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������ ����������� �����. 
        int init( unsigned int project_id );

        /// @brief ������������� ������������� �������� ����������.
        ///
        /// ���� ��� ���������� ���������� �� EEPROM ��������� ������ (����� 
        /// @ref init), ��������� ���������������� ���������� �� ��������� 
        /// ����� ������ �������� �������.
        ///
        /// @param auto_init_params - �������� �� ������� init_params �������
        /// tank � comb.
        /// @param auto_init_work_params - �������� �� ������� init_work_params 
        /// ������� tank � comb.
        /// @param custom_init_params_function - ���������������� ������� 
        /// ������������� ����������.
        void final_init( int auto_init_params = 1, 
            int auto_init_work_params = 1,
            void ( *custom_init_params_function )() = 0 );

        /// @brief ������ ���������� � EEPROM.
        ///
        /// ������ ���������� �� ������� ���������� � EEPROM.
        ///
        /// @param start_pos - ����� �������, � �������� ������ ������
        /// ���������� (��� ������ ������ ������ ���������).
        /// @param count - ���������� ������������ ����.
        void save( int start_pos = 0, int count = 0 );

        /// @brief ����� ����������� ����� (��� ������������� ���������� �� 
        /// ���������).
        void reset_CRC();

        /// @brief ��������� ��������� �� ���� ������ ����������.
        ///
        /// @param size      - ������ ����� ������ � ������.
        /// @param [out] start_pos - ��������� ������ � ���������� ������� 
        /// ����������. ��������� ��� ���������� ������ ���������� � ������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������ ����������� �����. 
        char* get_params_data( int size, int &start_pos );

    private:
        /// @brief �������� �����������. 
        /// 
        /// ��� ������ ������� ������������ ����������� ����� @ref get_instance.
        params_manager();

        /// ����������� ��������� ������ ��� ������ �������.
        static params_manager *instance;

        /// ������� ������ ����������.
        char params[ C_TOTAL_PARAMS_SIZE ];  

        /// ������������ ��� �������� ���������� ������ @ref parameters.
        int  last_idx; 

        /// ������� ���������� �������� ���������� (����� ����������� �����).
        int  loaded;

        unsigned int project_id;

        memory_range *params_mem;
        memory_range *CRC_mem;

        // ����������� ����������� �����.
        unsigned int solve_CRC();

        /// @brief �������� ����������� �����.
        ///
        /// ������� ����������� ����� � ��������� �� �� ���������, 
        /// ���������� � NVRAM (2 ������ ����� NVRAM).
        ///
        /// @return 0 - ��.
        /// @return 1 - ������ ����������� �����. 
        int check_CRC();

        /// @brief ������� ����������� ����� � ������ �� � NVRAM.
        void make_CRC();
    };
//-----------------------------------------------------------------------------
template < class type > class parameters
    {
    public:       
        type& operator[] ( unsigned int index );

        type get_val( int idx );

        int parse_cmd( char *buff );

        u_int get_count() const;
                
        parameters( int count, type *value = 0 );

    protected:
        type* get_values();

        // �������� ��� ��������� ����� ������ � ������� �� ��������.
        type            stub; 

        type            *values;
        unsigned int    count;
    };
//-----------------------------------------------------------------------------
class run_time_params_float: public parameters < float >, 
    array_device < float >
    {
    public:
        run_time_params_float( int count );

        float get_val( int idx ) const;

        int parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
class run_time_params_ulong: public parameters < u_int_4 >, 
    array_device < u_int_4 >
    {
    public:
        run_time_params_ulong( int count );

        u_int_4 get_val( int idx );

        int parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
template < class type > class saved_params:
public parameters < type > 
    {
    public:
        saved_params( int count );

        int parse_cmd( char *buff  );

        int save( u_int idx, type value );

    private:
        int start_pos;
    };
//-----------------------------------------------------------------------------
class saved_params_ulong: public saved_params < u_int_4 >, 
    array_device < u_int_4 >
    {
    public:
        saved_params_ulong( int count );

        u_int_4 get_val( int idx );

        int parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
class saved_params_float: public saved_params < float >, 
    array_device < float >
    {
    public:
        saved_params_float( int count );

        float get_val( int idx );

        int parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
class params_test
    {
    public:
        static int make_test();
    };
#endif // PARAMS_EX_H