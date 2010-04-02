/// @file param_ex.h
/// @brief �������� �������� �������, ������� ��������� ������ � �����������.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG - ���������� � ������� ���������� ���������� � �������.@n@n
/// @c USE_SIMPLE_DEV_ERRORS   - ���������� � ������� ������ ���������.@n
/// @c USE_NO_TANK_COMB_DEVICE - ���������� ��� �������� ���� � ��������. ��� 
/// ���������� ������� ��������� ��� ���������� ������� ���������������
/// ��������.@n@n
/// @c I7186_E - ���������� ��� PAC I7186_E.@n
/// @c I7188_E - ���������� ��� PAC I7188_E.@n
/// @c I7188   - ���������� ��� PAC I7188.@n
/// @c W750    - ���������� ��� PAC W750.@n
/// 
/// @par ������� ������:
/// @$Rev: $.\n
/// @$Author: $.\n
/// @$Date::                     $.

#ifndef PARAMS_EX_H
#define PARAMS_EX_H

#include <string.h>
#include "g_device.h"

#ifdef I7186_E
#include "plc_dev.h"
#endif // I7186_E

#ifdef W750
#include "PAC_dev.h"
#endif // I7186_E   

#include "sys.h"
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
/// @brief ������ � �������� ����������. 
/// 
/// ������ ��� �������� ���������� ����� ����������. ����������� ��������
/// ������� ����� ������ ( [] ).
template < class type > class parameters
    {
    public:      
        /// @brief ��������� �������� ����� �������� ��������������.
        ///
        /// @param index - ������ ��������.
        ///
        /// @return - �������� �������� � �������� @ref index. ���� ������ 
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// stub ( �������� 0 ).
        type& operator[] ( unsigned int index )
            {
            if ( index < count )
                {
                return values[ index ];
                }
#ifdef DEBUG
            else
                {
                Print( "parameters[] - error: index[ %u ] > count [ %u ]\n",
                    index, count );
                }
#endif // DEBUG

            stub = 0;
            return stub;
            }

        /// @brief ��������� �������� ����� ������.
        ///
        /// ��� ���������� ���������� ������ @ref array_device.
        ///
        /// @param index - ������ ��������.
        ///
        /// @return - �������� �������� � �������� @ref index. ���� ������ 
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// stub ( �������� 0 ).
        type get_val( int idx )
            {
            return this->operator []( idx );
            }

        /// @brief ���������� �������.
        ///
        /// ��� ���������� ���������� ������ @ref array_device.
        ///
        /// @param buff - ���������� ����� � ��������.
        ///
        /// @return - ���������� ������������ ���� �������.
        int parse_cmd( char *buff )
            {
            int par_n = *( ( u_int_4* ) buff );
            ( *this )[ par_n ] = *( ( type* ) ( buff + sizeof( u_int_4 ) ) );

            return sizeof( u_int_4 ) + sizeof( type );
            }

        /// @brief ��������� ���������� ����������.
        ///
        /// @return - ���������� ����������.
        u_int get_count()
            {
            return count;
            }

        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        /// @param value - ��������� �� ����� ��� �������� �������� ����������.
        parameters( int count, type *value = 0 ): count( count ),
            values( value )
            {
#ifdef DEBUG
            if ( 0 == count )
                {
                Print( "parameters(...) - error: count = 0!\n" );
                }
#endif // DEBUG
            if ( count > 0 && 0 == values )
                {
                values = new type[ count ];
                memset( values, 0, count * sizeof( type ) );
                }
            }

    protected:
        /// @brief ��������� ��������� �� ����� ��� �������� ��������
        /// ����������.
        ///
        /// @return - ��������� �� ����� ��� �������� �������� ����������.
        type* get_values()
            {
            return values;
            }

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
        run_time_params_float( int count ):parameters < float >( count ),
            array_device < float >( 1, 
            "RT_PARAM_F", 
            count, 
            i_complex_device::ARRAY_DEV_FLOAT )
            {
            }

        float get_val( int idx )
            {
            return parameters< float >::get_val( idx );
            }

        int parse_cmd( char *buff )
            {
            int res = parameters< float >::parse_cmd( buff );
#ifdef DEBUG

#ifdef W750
            Print( "Set val work param float[ %u ] = %f\n",
                *( ( u_int_4* ) buff ), 
                *( ( float* ) ( buff + sizeof( u_int_4 ) ) ) );
#else
            Print( "Set val work param float[ %lu ] = %f\n",
                *( ( u_int_4* ) buff ), 
                *( ( float* ) ( buff + sizeof( u_int_4 ) ) ) );
#endif // W750

#endif //DEBUG
            return res;
            }

    };
//-----------------------------------------------------------------------------
class run_time_params_ulong: public parameters < u_int_4 >, 
    array_device < u_int_4 >
    {
    public:
        run_time_params_ulong( int count ) : parameters < u_int_4 >( count ),
            array_device < u_int_4 >( 1, "RT_PARAM_UL", count, 
            i_complex_device::ARRAY_DEV_ULONG )
            {
            }

        u_int_4 get_val( int idx )
            {
            return parameters< u_int_4 >::get_val( idx );
            }

        int parse_cmd( char *buff )
            {
            int res = parameters< u_int_4 >::parse_cmd( buff );
#ifdef DEBUG

#ifdef W750
            Print( "Set val work param ulong[ %u ] = %u\n",
                *( ( u_int_4* ) buff ), 
                *( ( u_int_4* ) ( buff + sizeof( u_int_4 ) ) ) );
#else
            Print( "Set val work param ulong[ %lu ] = %lu\n",
                *( ( u_int_4* ) buff ), 
                *( ( u_int_4* ) ( buff + sizeof( u_int_4 ) ) ) );
#endif // W750

#endif //DEBUG
            return res;
            }

    };
//-----------------------------------------------------------------------------
template < class type > class saved_params:
public parameters < type > 
    {
    public:
        saved_params( int count ) : parameters < type >( 
            count,
            ( type* ) params_manager::get_instance()->get_params_data( 
            count * sizeof( type ), start_pos ) )
            {
            }
        //---------------------------------------------------------------------
        int parse_cmd( char *buff )
            {
            int res = parameters< type >::parse_cmd( buff );

            u_int_4 idx = *( ( u_int_4* ) buff );

            params_manager::get_instance()->save( 
                start_pos + idx, sizeof( u_int_4* ) );

            return res;
            }
        //---------------------------------------------------------------------
        int save( u_int idx, type value )
            {
            int res = 1;
            if ( idx < parameters< type >::get_count() )
                {
                parameters< type >::get_values()[ idx ] = value;

                params_manager::get_instance()->save( 
                    start_pos + idx * sizeof( type ), sizeof( type ) );
                res = 0;
                }
#ifdef DEBUG
            else
                {
                Print( "parameters:save - index[ %u ] > count [ %u ]\n",
                    idx, parameters< type >::get_count() );
                }
#endif // DEBUG
            return res;
            }

    private:
        int start_pos;
    };
//-----------------------------------------------------------------------------
class saved_params_ulong: public saved_params < u_int_4 >, 
    array_device < u_int_4 >
    {
    public:
        saved_params_ulong( int count ) :
          saved_params < u_int_4 >( count ),
              
              array_device < u_int_4 >( 1, "S_PARAM_UL", count, 
              i_complex_device::ARRAY_DEV_ULONG )
              {
              }

          u_int_4 get_val( int idx )
              {
              return saved_params< u_int_4 >::get_val( idx );
              }

          int parse_cmd( char *buff )
              {
              int res = saved_params< u_int_4 >::parse_cmd( buff );
#ifdef DEBUG

#ifdef W750
              Print( "Set val saved param ulong[ %u ] = %u\n",
                  *( ( u_int_4* ) buff ), 
                  *( ( u_int_4* ) ( buff + sizeof( u_int_4 ) ) ) );
#else
              Print( "Set val saved param ulong[ %lu ] = %lu\n",
                  *( ( u_int_4* ) buff ), 
                  *( ( u_int_4* ) ( buff + sizeof( u_int_4 ) ) ) );
#endif // W750

#endif //DEBUG
              return res;
              }
    };
//-----------------------------------------------------------------------------
class saved_params_float: public saved_params < float >, 
    array_device < float >
    {
    public:
        saved_params_float( int count ):
          saved_params < float >( count ),
              array_device < float >( 1, "S_PARAM_F", count, 
              i_complex_device::ARRAY_DEV_FLOAT )
              {
              }

          float get_val( int idx )
              {
              return saved_params< float >::get_val( idx );
              }

          int parse_cmd( char *buff )
              {
              int res = saved_params< float >::parse_cmd( buff );
#ifdef DEBUG
              
#ifdef W750
              Print( "Set val saved param float[ %u ] = %f\n",
                  *( ( u_int_4* ) buff ), 
                  *( ( float* ) ( buff + sizeof( u_int_4 ) ) ) );
#else
              Print( "Set val saved param float[ %lu ] = %f\n",
                  *( ( u_int_4* ) buff ), 
                  *( ( float* ) ( buff + sizeof( u_int_4 ) ) ) );
#endif // W750
              
#endif //DEBUG
              return res;
              }
    };
//-----------------------------------------------------------------------------
class params_test
    {
    public:
        static int make_test();
    };
#endif // PARAMS_EX_H
