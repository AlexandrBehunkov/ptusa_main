/// @file param_ex.h
/// @brief �������� �������� �������, ������� ��������� ������ � �����������.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG - ���������� � ������� ���������� ���������� � �������.@n
/// @c KEY_CONFIRM - ������� � ���������� ����������� ��������� �� �������
/// �������.@n@n
/// @c USE_SIMPLE_DEV_ERRORS   - ���������� � ������� ������ ���������.@n
/// @c USE_NO_TANK_COMB_DEVICE - ���������� ��� �������� ���� � ��������. ��� 
/// ���������� ������� ��������� ��� ���������� ������� ���������������
/// ��������.
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef PARAMS_EX_H
#define PARAMS_EX_H

#include <string.h>

#include "sys.h"
#ifdef LINUX_OS
#include "sys_linux.h"
#endif // LINUX_OS

#ifdef WIN_OS
#include "sys_win.h"
#endif // WIN_OS
#include "PAC_dev.h"
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

        ~params_manager();

    private:
        /// @brief �������� �����������. 
        /// 
        /// ��� ������ ������� ������������ ����������� ����� @ref get_instance.
        params_manager();

        /// ����������� ��������� ������ ��� ������ �������.
        static auto_smart_ptr< params_manager > instance;

        /// ������� ������ ����������.
        char params[ C_TOTAL_PARAMS_SIZE ];  

        /// ����� ���������� ����������� ���������. ������������ ��� ��������
        /// ���������� ������ @ref parameters.
        u_int last_idx;

        /// ������� ���������� �������� ���������� (������������� �����������
        /// �����).
        int loaded;

        u_int project_id;   ///< ����� ������� (��� ������������ ����������).

        memory_range *params_mem; ///< ������ ����������.
        memory_range *CRC_mem;    ///< ������ ����������� �����.

        // ������������ ����������� �����.
        u_int solve_CRC();

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
        /// @return - �������� �������� � �������� ��������. ���� ������ 
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

        int save( u_int idx, type val )
            {
            this->operator []( idx ) = val;
            return 0;
            }

        /// @brief ��������� �������� ����� ������.
        ///
        /// ��� ���������� ���������� ������ @ref array_device.
        ///
        /// @param idx - ������ ��������.
        ///
        /// @return - �������� �������� � �������� ��������. ���� ������ 
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
            u_int_4 par_n;
            memcpy( &par_n, buff, sizeof( par_n ) );
            buff += sizeof( par_n );

            memcpy( &( ( *this )[ par_n ] ), buff, sizeof( type ) );

            return sizeof( par_n ) + sizeof( type );
            }

        /// @brief ���������� ��� �������� ���������� � 0.
        virtual void reset_to_0()
            {
            for ( u_int i = 0; i < count; i++ )
                {
                values[ i ] = 0;
                }
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
            is_delete = 0;
            if ( count > 0 && 0 == values )
                {
                is_delete = 1;
                values = new type[ count ];
                memset( values, 0, count * sizeof( type ) );
                }
            }

        virtual ~parameters()
            {
            if ( 1 == is_delete && count > 0 && values > 0 )
                {
                delete [] values;
                values = 0;
                count = 0;
                }
            }

    protected:
        char is_delete; ///< ������� �������� ������ ��� �������� �������.

        /// @brief ��������� ��������� �� ����� ��� �������� ��������
        /// ����������.
        ///
        /// @return - ��������� �� ����� ��� �������� �������� ����������.
        type* get_values()
            {
            return values;
            }

        /// �������� ��� ��������� ����� ������ � ������� �� ��������.
        type         stub; 

        unsigned int count;     ///< ���������� ���������.
        type         *values;   ///< ��������� �� ������ �������� ���������.
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������� ������� ���������� ���� float.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����.
class run_time_params_float: public parameters < float >,
    public array_device < float >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        run_time_params_float( int count ):parameters < float >( count ),
            array_device < float >( 1,
            "RT_PARAM_F", 
            count, 
            i_complex_device::ARRAY_DEV_FLOAT )
            {
            }

        /// @brief ���������� ���������� ������ @ref array_device.
        float get_val( int idx )
            {
            return parameters< float >::get_val( idx );
            }

        /// @brief ���������� ���������� ������ @ref array_device.
        int parse_cmd( char *buff )
            {
            int res = parameters< float >::parse_cmd( buff );
#ifdef DEBUG
            float   val;
            u_int_4 idx;
            memcpy( &idx, buff, sizeof( idx ) );
            buff += sizeof( idx );
            memcpy( &val, buff, sizeof( val ) );

            Print( "Set val work param float[ %2u ] = %f\n", idx, val );
#endif // DEBUG
            return res;
            }

    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������� ������� ���������� ���� @ref u_int_4.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����.
class run_time_params_u_int_4: public parameters < u_int_4 >,
    public array_device < u_int_4 >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        /// @param name  - ��� ����������.
        run_time_params_u_int_4( int count,
            const char* name = "RT_PARAM_UL" ) : parameters < u_int_4 >( count ),
            array_device < u_int_4 >( 1, name, count,
            i_complex_device::ARRAY_DEV_ULONG )
            {
            }

        virtual ~run_time_params_u_int_4()
            {
            }

        /// @brief ���������� ���������� ������ @ref array_device.
        u_int_4 get_val( int idx )
            {
            return parameters< u_int_4 >::get_val( idx );
            }

        /// @brief ���������� ���������� ������ @ref array_device.
        int parse_cmd( char *buff )
            {
            int res = parameters< u_int_4 >::parse_cmd( buff );
#ifdef DEBUG
            u_int_4 val;
            u_int_4 idx;
            memcpy( &idx, buff, sizeof( idx ) );
            buff += sizeof( idx );
            memcpy( &val, buff, sizeof( val ) );

            Print( "Set val work param u_int_4[ %2u ] = %u\n", idx, val );
#endif // DEBUG
            return res;
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ �����������.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����. ���
/// ������������ PAC �� �������� �����������.
template < class type > class saved_params:
public parameters < type > 
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params( int count ) : parameters < type >(
            count,
            ( type* ) params_manager::get_instance()->get_params_data( 
            count * sizeof( type ), start_pos ) )
            {
            }

        virtual ~saved_params()
            {
            }

        /// @brief ���������� ���������� ������ @ref array_device.
        int parse_cmd( char *buff )
            {
            int res = parameters< type >::parse_cmd( buff );

            u_int_4 idx;
            memcpy( &idx, buff, sizeof( idx ) );

            params_manager::get_instance()->save( 
                start_pos + idx * sizeof( type ), sizeof( type ) );

            return res;
            }

        /// @brief ���������� �������� ��������� � ����������������� ������.
        ///
        /// �������� ������� ����� ������ ��������� �������� ��������� ������
        /// � ������, ��� ��� ��� ���������� ��� � ����������������� ������ ����
        /// ������������ ������ �����.
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

        /// @brief ���������� �������� ���� ��������� � ����������������� ������.
        ///
        /// �������� ������� ����� ������ ��������� �������� ��������� ������
        /// � ������, ��� ��� ��� ���������� ��� � ����������������� ������ ����
        /// ������������ ������ �����.
        int save_all()
            {
            params_manager::get_instance()->save(
                start_pos,  parameters< type >::get_count() * sizeof( type ) );

            return 0;
            }

        void reset_to_0()
            {
            for ( u_int i = 0; i <  parameters< type >::get_count(); i++ )
                {
                parameters< type >::get_values()[ i ] = 0;
                }

            params_manager::get_instance()->save(
                start_pos, sizeof( type ) * parameters< type >::get_count() );
            }

    private:
        /// ������ ������ �������� � ����� �������, ��� ���������� ��������
        /// ��������� � ����������������� ������ (@ref save).
        int start_pos;
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ ����������� ���� @ref u_int_4.
class saved_params_u_int_4: public saved_params < u_int_4 >,
    public array_device < u_int_4 >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params_u_int_4( int count ) :
          saved_params < u_int_4 >( count ),              
              array_device < u_int_4 >( 1, "S_PARAM_UL", count,
              i_complex_device::ARRAY_DEV_ULONG )
              {
              }

          /// @brief ���������� ���������� ������ @ref array_device.
          u_int_4 get_val( int idx )
              {
              return saved_params< u_int_4 >::get_val( idx );
              }

          /// @brief ���������� ���������� ������ @ref array_device.
          int parse_cmd( char *buff )
              {
              int res = saved_params< u_int_4 >::parse_cmd( buff );
#ifdef DEBUG
              u_int_4  val;
              u_int_4 idx;
              memcpy( &idx, buff, sizeof( idx ) );
              buff += sizeof( idx );
              memcpy( &val, buff, sizeof( val ) );

              Print( "Set val saved param u-int_4[ %2u ] = %u\n", idx, val );
#endif // DEBUG

              return res;
              }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ ����������� ���� float.
class saved_params_float: public saved_params < float >, 
    public array_device < float >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params_float( int count ):
          saved_params < float >( count ),
              array_device < float >( 1, "S_PARAM_F", count, 
              i_complex_device::ARRAY_DEV_FLOAT )
              {
              }

          virtual ~saved_params_float()
              {
              }

          /// @brief ���������� ���������� ������ @ref array_device.
          float get_val( int idx )
              {
              return saved_params< float >::get_val( idx );
              }

          /// @brief ���������� ���������� ������ @ref array_device.
          int parse_cmd( char *buff )
              {
              int res = saved_params< float >::parse_cmd( buff );
#ifdef DEBUG
              float   val;
              u_int_4 idx;
              memcpy( &idx, buff, sizeof( idx ) );
              buff += sizeof( idx );
              memcpy( &val, buff, sizeof( val ) );

              Print( "Set val saved param float[ %2u ] = %f\n", idx, val );
#endif //DEBUG
              return res;
              }
    };
//-----------------------------------------------------------------------------
/// @brief ����� ��� ������������ ������� ������ � �����������.
class params_test
    {
    public:
        /// @brief ������������ ������� ������ � �����������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������.
        static int make_test();
    };
//-----------------------------------------------------------------------------
#endif // PARAMS_EX_H
