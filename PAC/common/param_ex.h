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

#include "mem.h"
#include "g_device.h"
//-----------------------------------------------------------------------------
/// @brief ������ � �����������. 
/// 
/// ������ ��� �������� �������� ��� ������ � �����������. ����������� 
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

        /// @brief ��������� ���� ���������� � ������� ��������.
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

        void print() const
            {
            printf( "param %d\n", count );
            }
        
        virtual int save_device( char *buff, const char *prefix ) = 0;

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
class run_time_params_float: public parameters < float >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        run_time_params_float( int count ):parameters < float >( count )
            {
            }

        int save_device( char *buff, const char *prefix )
            {
            sprintf( buff, "%sRT_PAR_F = {}\n\r", prefix );
            int answer_size = strlen( buff );

            for ( u_int i = 0; i < count; i++ )
                {
                sprintf( buff + answer_size, "%sRT_PAR_F[%d] = %.2f\n\r", 
                    prefix, i + 1, get_val( i ) );

                answer_size += strlen( buff + answer_size );
                }

            return answer_size;
            }

    protected:
        /// @brief ���������� ���������� ������ @ref array_device.
        float get_val( int idx )
            {
            return parameters< float >::get_val( idx );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������� ������� ���������� ���� @ref u_int_4.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����.
class run_time_params_u_int_4: public parameters < u_int_4 >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        /// @param name  - ��� ����������.
        run_time_params_u_int_4( int count,
            const char* name = "RT_PARAM_UL" ) : parameters < u_int_4 >( count )           
            {
            }

        virtual ~run_time_params_u_int_4()
            {
            }

        int save_device( char *buff, const char *prefix )
            {
            sprintf( buff, "%sRT_PAR_UI = {}\n\r", prefix );
            int answer_size = strlen( buff );

            for ( u_int i = 0; i < count; i++ )
                {
                sprintf( buff + answer_size, "%sRT_PAR_UI[%d] = %lu\n\r", 
                    prefix, i + 1, ( unsigned long int ) get_val( i ) );

                answer_size += strlen( buff + answer_size );
                }

            return answer_size;
            }

    protected:
        /// @brief ���������� ���������� ������ @ref array_device.
        u_int_4 get_val( int idx )
            {
            return parameters< u_int_4 >::get_val( idx );
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

        /// @brief ���������� �������� ���� ���������� � ����������������� ������.
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

        /// @brief ��������� ���� ���������� � ������� �������� � ����������
        /// � ����������������� ������.
        void reset_to_0()
            {
            for ( u_int i = 0; i <  parameters< type >::get_count(); i++ )
                {
                parameters< type >::get_values()[ i ] = 0;
                }

            params_manager::get_instance()->save(
                start_pos, sizeof( type ) * parameters< type >::get_count() );
            }

        void print() const
            {
            //printf( "saved param %d\n", count );
            }

    private:
        /// ������ ������ �������� � ����� �������, ��� ���������� ��������
        /// ��������� � ����������������� ������ (@ref save).
        int start_pos;
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ ����������� ���� @ref u_int_4.
class saved_params_u_int_4: public saved_params < u_int_4 >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params_u_int_4( int count ) :
          saved_params < u_int_4 >( count )
              {
              }

          virtual ~saved_params_u_int_4()
              {
              }

          //int save_device( char *buff, char *prefix )
          //    {
          //    sprintf( buff, "%sPAR_UI = {}\n\r", prefix );
          //    int answer_size = strlen( buff );

          //    for ( u_int i = 0; i < count; i++ )
          //        {
          //        sprintf( buff + answer_size, "%sPAR_UI[%d] = %lu\n\r", 
          //            prefix, i + 1, get_val( i ) );

          //        answer_size += strlen( buff + answer_size );
          //        }

          //    return answer_size;
          //    }

          int save_device( char *buff, const char *prefix )
              {
              sprintf( buff, "%sPAR_UI = \n%s\t{\n", prefix, prefix );
              int answer_size = strlen( buff );

              for ( u_int i = 0; i < count; i++ )
                  {
                  sprintf( buff + answer_size, "%s\t[%d] = %u,\n", 
                      prefix, i + 1, get_val( i ) );

                  answer_size += strlen( buff + answer_size );
                  }

              sprintf( buff + answer_size, "%s\t},\n", prefix );
              answer_size += strlen( buff + answer_size );
              return answer_size;
              }

    protected:
        /// @brief ���������� ���������� ������ @ref array_device.
        u_int_4 get_val( int idx )
            {
            return saved_params< u_int_4 >::get_val( idx );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ ����������� ���� float.
class saved_params_float: public saved_params < float >     
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params_float( int count ):
          saved_params < float >( count )
              {
              }

          virtual ~saved_params_float()
              {
              }

          int save_device( char *buff, const char *prefix )
              {
              sprintf( buff, "%sPAR_F = \n%s\t{\n", prefix, prefix );
              int answer_size = strlen( buff );

              sprintf( buff + answer_size, "%s\t", prefix );
              answer_size += strlen( buff + answer_size );

              for ( u_int i = 0; i < count; i++ )
                  {
                  float val  = get_val( i );
                  //sprintf( buff + answer_size, "%s\t[%d] = %.2f,\n", 
                  //   prefix, i + 1, get_val( i ) );
                  if ( 0 == val )
                      {
                      sprintf( buff + answer_size, "0, " );
                      }
                  else
                      {         
                      double tmp;
                      if ( modf( val, &tmp ) == 0 )
                          {
                          sprintf( buff + answer_size, "%d, ", ( int ) val );
                          }
                      else
                          {
                          sprintf( buff + answer_size, "%.2f, ", val );
                          }                      
                      }

                  answer_size += strlen( buff + answer_size );
                  }

              sprintf( buff + answer_size, "\n%s\t},\n", prefix );
              answer_size += strlen( buff + answer_size );
              return answer_size;
              }

    protected:
        /// @brief ���������� ���������� ������ @ref array_device.
        float get_val( int idx )
            {
            return saved_params< float >::get_val( idx );
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
