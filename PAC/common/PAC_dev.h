/// @file PAC_dev.h
/// @brief ������, ������� ��������� ������� �������� 
/// ��������� ��������� PAC �� ������. ����� ����������� ������ ��� ������ � 
/// ������������ ( �������, ������,... ). ������ ������������ ������ � 
/// ����������� ( PAC ).
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG - ���������� c ������� ���������� ���������� � �������.
/// @c DEBUG_NO_WAGO_MODULES - ������� ���������� �������� ��� �������
/// wago (��������� ������ � ����).
/// 
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef PAC_DEVICES_H
#define PAC_DEVICES_H

#include <string.h>
#include <stdio.h>

#include <vector>

#include "sys.h"
#include "wago.h"
#include "g_device.h"

class saved_params_u_int_4;

//for levels
#if defined LEVEL_REVERSE
#define LIS     1
#define LNO     0
#else
#define LIS     0
#define LNO     1
#endif

#define OFF     0
#define ON      1

//-----------------------------------------------------------------------------
/// @brief ������ ������, ������� ������������ ��� �������� ��������� ���������,
/// ������� �������������� �������� ���������� ����.
//
/// ��������: ���������, ��������� � �.�.
template < class data_type > class array_device: public i_simple_device
    {
    public:
        /// @param n        - �����.
        /// @param new_name - ���.
        /// @param new_subdev_cnt - ���������� ��������� � �������.
        /// @param type     - ���. ��� ���������� ��������� � �������� ��
        /// ������.
        array_device( u_int_4 n, 
            const char *new_name,
            u_int_2 new_subdev_cnt,
            char type ): sub_dev_cnt( new_subdev_cnt ), 
            type( type ), 
            n( n )
            {
            name = new char[ strlen( new_name ) + 1 ];
            strcpy( name, new_name );

            prev_val = new data_type[ sub_dev_cnt ];
            for ( unsigned int i = 0; i < sub_dev_cnt; i++ )
                {
                prev_val[ i ] = 0;
                }              
            }

        virtual ~array_device()
            {
            if ( name )
                {
                delete [] name;
                name = 0;
                }
            if ( prev_val )
                {
                delete [] prev_val;
                prev_val = 0;
                }
            }

        /// @brief ���������� ���������� � �������� �����.
        ///
        /// ��� �������� ���������� �� ������.
        ///
        /// @param buff [out] - ��������� �� �������� �����, ���� �����
        /// ����������� ���������� ����������.
        ///
        /// @return - ���������� ����������� ����.
        int  save_device( char *buff )
            {
            // ������ ������ (buff) � ��������� ����:
            //    1 ����  - ���;                                (1)
            //    4 ����� - �����;                              (2)
            //    1 ����  - ����� ����� ������ ����������;      (3)
            //    � ����  - ��� ������ ����������;              (4)
            //    4 ����� - ���������� ������������;            (5)

            u_int_2 idx = 0;

            buff[ idx++ ] = type;                               //(1)
            memcpy( buff + idx, &n, sizeof( n ) );              //(2)
            idx += sizeof( n );
            buff[ idx++ ] = strlen( name );                     //(3)
            strcpy( buff + idx, name );                         //(4)
            idx += strlen( name ) + 1;
            memcpy( buff + idx, &sub_dev_cnt, sizeof( sub_dev_cnt ) );  //(5)
            idx += sizeof( sub_dev_cnt );

            return idx;
            }

        /// @brief ���������� ��������� ���������� � �������� �����.
        ///
        /// ��� �������� ��������� ���������� �� ������.
        ///
        /// @param buff [out] - ��������� �� �������� �����, ���� �����
        /// ����������� ���������� ��������� ����������.
        ///
        /// @return - ���������� ����������� ����.
        int  save_state( char *buff )
            {
            // ������ ������ (buff) � ��������� ����:
            //  4 ����� - ����� ����������;                     (1)
            //  4 ����� - ���������� ������������;              (2)
            //  �����   - ������ ������� �������������.
            u_int_2 answer_size = 0;
            memcpy( buff + answer_size, &n, sizeof( n ) );                    //(1)
            answer_size += sizeof( n );
            memcpy( buff + answer_size, &sub_dev_cnt, sizeof( sub_dev_cnt ) );//(2)
            answer_size += sizeof( sub_dev_cnt );

            for ( u_int_4 i = 0; i < sub_dev_cnt; i++ )
                {
                data_type val = get_val( i );
                memcpy( buff + answer_size, &val, sizeof( val ) );

                prev_val[ i ] = val;
                answer_size += sizeof( data_type );      
                }      

            return answer_size;
            }

        /// @brief ���������� ������������� ��������� ���������� � �������� �����.
        ///
        /// ��� �������� ��������� ���������� �� ������.
        ///
        /// @param buff [out] - ��������� �� �������� �����, ���� �����
        /// ����������� ���������� ��������� ����������.
        ///
        /// @return - ���������� ����������� ����.
        int  save_changed_state( char *buff )
            {
            // ������ ������ (buff) � ��������� ����:          
            //  2 ����� - ���������� ���������, ���������� ���� ���������;  (1)
            //  2 ����� - ����� ���������� � ������� ���������.             (2)
            //  �����   - ��� ���������� ���������                          (3)
            // ��������� ���������� �������� ��� ���������� ��������.       (4)
            //�������� ������ ������.                                       (5)
            //����������� �� 1 ���������� ���������, ���������� ����
            //���������.                                                    (6)
            //���� ��� ���������, ���������� ���� ���������, ���������� 0.  (7)
            u_int_2 *changed_dev_cnt = ( u_int_2* ) ( buff );               //1
            *changed_dev_cnt = 0;
            u_int_2 answer_size = 2;

            for ( u_int_2 i = 0; i < sub_dev_cnt; i++ )
                {
                if ( get_val( i ) != prev_val[ i ] )
                    {
                    ( ( u_int_2* ) ( buff + answer_size ) )[ 0 ] = i;       //2
                    answer_size += 2;
                    ( ( data_type* ) ( buff + answer_size ) )[ 0 ] =        //3
                        get_val( i );
                    prev_val[ i ] = get_val( i );                           //4
                    answer_size += sizeof( data_type );                     //5
                    ( *changed_dev_cnt )++;                                 //6
                    }
                }
            if ( 2 == answer_size )                                         //7
                {
                return 0;
                }

            return answer_size;              
            }

        /// @brief ��������� �������� �������� ����� ������. 
        ///
        /// ��� �������� �����, ��������, ����� ������������ ��� ���������� 
        /// ������� � ����� ( @ref save_state ).
        ///
        /// @param idx - ������ ��������.
        ///
        /// @return - �������� �������� � �������� ��������.
        virtual data_type get_val( int idx ) = 0;

        /// @brief ����� ������� � �������.
        ///
        /// ��� ������������� � ���������� �����.
        void print() const
            {
            char tmp_str[ 100 ];    

            if ( strlen( name ) < 8 ) 
                {
                sprintf( tmp_str, "\"%s\", \t\t[ %3lu ]", 
                    name, ( unsigned long int ) sub_dev_cnt );
                }
            else sprintf( tmp_str, "\"%s\", \t[ %3lu ]", 
                name, ( unsigned long int ) sub_dev_cnt );

            print_str( tmp_str );
            }

        /// @brief ����������-�������� ���������� ������ @ref i_simple_device.
        u_int_4 get_n() const
            { 
            return 0; 
            } 

    protected:
        u_int_4         sub_dev_cnt;    ///< ���������� ������������.
        char            *name;          ///< ���.
        char            type;           ///< ���.
        u_int_4         n;              ///< ���������� �����.

        data_type*      prev_val;       ///< ������ ���������� ��������.
    };
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//����� ��������.
//  ������ ��� ������������� ��������� ������� �������� (����, ��������).
template < class data_type > class device_state:
public array_device < data_type >
    {
    public:
        enum OBJECT_TYPE
            {
            T_TECH_OBJECT = 1,
            T_PID,
            T_CNTR,
            T_MNGR,
            T_PACK_DEVICE,
            T_F_DEVICE,
            T_MSA,

            T_PATH_COMB,   //�������� ���������.
            };

        device_state( u_int_4 n,
            const char *new_name,
            u_int_2 new_subdev_cnt,
            char type,
            u_int_4 *state,
            void *owner_object,
            char owner_type ):
        array_device < data_type >( n,
            new_name,
            new_subdev_cnt,
            type ),                                       
            state( state ),
            owner_object( owner_object ),
            owner_type( owner_type )
            {
            }

        virtual ~device_state()
            {
            }

    protected:
        u_int_4     *state;
        void        *owner_object;
        char        owner_type;
    };
//-----------------------------------------------------------------------------
//����� ��������.
//  ������ ��� ������������� ��������� ������� �������� (����, ��������).
//  ������ ����� ���������� ����� 1 ����.
class single_state: public device_state < char >
    {
    public:
        single_state( const char *name, int n, u_int_4 *state,
            void *owner_object, char owner_type, int size ):
        device_state < char >( n, name, size,
            i_complex_device::ARRAY_DEV_BYTE,
            state,
            owner_object,
            owner_type )
            {
            }

        virtual ~single_state()
            {            
            }

        char get_val( int idx );
        int  parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
//����� ��������.
//  ������ ��� ������������� ��������� ������� �������� (����, ��������).
//  ������ 32 ������ ���������� ��� ���� ����� (4 �����).
class complex_state: public device_state < u_int_4 >
    {
    public:
        complex_state( const char *name, int n, u_int_4 *state,
            void *owner_object, char owner_type, int size = 1 );

        virtual ~complex_state()
            {
            }

        u_int_4 get_val( int idx );
        int     parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//����� ��������.
//  ������ ������, ������� ������������ ��� �������� �����.
class string_device: public i_simple_device
    {
    protected:
        char    *name;          // ���.
        u_int_4 n;              // ���������� �����.
        char    *str;           // ������.
        int     max_str_len;    // ������������ ����� ������, ��� set_value().

    public:
        string_device( u_int_4 n, const char *new_name, char* str, int max_str_len );

        int  save_device( char *buff );
        int  save_state( char *buff );
        int  save_changed_state( char *buff );

        void    print() const;
        u_int_4 get_n() const;

        int     parse_cmd( char *buff  );
    };
//-----------------------------------------------------------------------------
//����� ��������.
//  ��������� ���������� � ������������ ������.
class var_state:public i_simple_device
    {    
    public:
        var_state( const char *new_name, char type,
            u_int_4 &var ):var( var ),
            type( type )
            {            
            strlcpy( name, new_name, C_MAX_NAME_LENGTH );
            }

        int parse_cmd( char *buff  )
            {
            var = ( ( u_int_4* ) ( buff + 4 ) )[ 0 ];
            return 4 + 4;
            }

        int save_device( char *buff )
            {
            // ������ ������ (buff) � ��������� ����:
            //    1 ����  - ���;                              (1)
            //    4 ����� - �����;                            (2)
            //    1 ����  - ����� ����� ������ ����������;    (3)
            //    � ����  - ��� ������ ����������;            (4)
            //    4 ����� - ���������� ������������;          (5)
            u_int_2 idx = 0;

            buff[ idx++ ] = type;                             //(1)                            
            ( ( u_int_4* ) ( buff + idx ) )[ 0 ] = 1;         //(2)
            idx += 4;
            buff[ idx++ ] = strlen( name );                   //(3)              
            strcpy( buff + idx, name );                       //(4)
            idx += strlen( name ) + 1;
            ( ( u_int_4* ) ( buff + idx ) )[ 0 ] = 1;         //(5)
            idx += 4;           

            return idx;
            }

        int save_state( char *buff )
            {
            // ������ ������ (buff) � ��������� ����:
            //  4 ����� - ����� ����������;           (1)
            //  4 ����� - ���������� ������������;    (2)
            //  4 ����� - ������ ����������.          (3)  

            ( ( u_int_4* ) buff )[ 0 ] = 1;           //(1)
            ( ( u_int_4* ) buff )[ 1 ] = 1;           //(2)

            ( ( u_int_4* ) buff )[ 2 ] = var;         //(3)
            return 4 + 4 + 4;
            }

        int save_changed_state( char *buff )
            {
            return save_state( buff );
            }

        void print() const
            {
            char tmp_str[ 100 ];    
            sprintf( tmp_str, "\"%s\" ", name );

            print_str( tmp_str );
            }

        u_int_4 get_n() const
            {
            return 1;
            }

        int load_state( char *buff )
            {
            buff++;
            return 0;
            }

        int load_changed_state( char *buff )
            {
            buff++;
            return 0;
            }

        int load_device( char *buff )
            {
            buff++;
            return 0;
            }

        u_int_4 get_idx()
            {
            return 1;
            }

        void set_idx( u_int_4 new_idx )
            {
            new_idx++;
            }

    private:
        enum CONSTANTS
            {
            C_MAX_NAME_LENGTH = 20,
            };

        u_int_4 &var;
        char    type;
        char    name[ C_MAX_NAME_LENGTH ];          //���.
    };
//-----------------------------------------------------------------------------
/// @brief ��������� ��������.
class i_counter
    {
    public:
        /// @brief ������������ ������ ��������.
        virtual void pause() = 0;

        /// @brief ������������� ������ ��������.
        virtual void start() = 0;

        /// @brief ����� �������� � ��������� �����.
        /// 
        /// ����� ������ ��� ����������� ������ ���������� ������� @ref start().
        virtual void reset() = 0;

        /// @brief ����� �������� � ����������� �����.
        void restart();

        /// @brief ��������� �������� ��������.
        virtual u_int get_quantity() = 0;

        virtual ~i_counter()
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� �����.
///
/// �������� �����, ���������� ������� � �.�. �������� ��������� �����
/// ���������.
class i_DI_device
    {
    public:
        i_DI_device();

        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @return - ������� ��������� ���������� � ���� ������ �����.
        virtual int get_state_now() = 0;

        /// @brief ��������� ���������������� ��������� ����������.
        ///
        /// ���������� ������ ���� ���������, ���� ���������� ���� ��������� �
        /// � ������ ��������� ������ ��������� ��������� ������� (�������
        /// ��������� - ������ @ref set_change_time).
        ///
        /// @return - ��������� ���������� � ���� ������ �����.
        virtual int get_state();

        /// @brief ��������� ������� �������� ��������� ���������.
        ///
        /// @param time - ����� �������� ��������� ���������.
        void set_change_time( u_int time );

        /// @brief ��������� ���������.
        ///
        /// ������ ����� ������������ ��� ������� ��������� ���������� �����
        /// ��� ���������.
        ///
        /// @param new_state - ����� ���������.
        void set_state( int new_state );

    protected:
        u_int last_check_time;///< ����� ��������� �������� ���������.
        int   state;          ///< ���������� ���������, ��� ���������� ��������.
        u_int dt;             ///< �������� ������������ ���������, ����.
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� ������.
///
/// ������, ������� � �.�. �������� ��������� ����� ���������.
class i_DO_device: public i_DI_device
    {
    public:
        /// @brief ��������� ����������.
        ///
        /// ��������� ���������� � �������� ���������. ��� ������� ��� ��������
        /// ��� �������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void on() = 0;

        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;

        /// @brief ��������� ������ ��������� ����������.
        ///
        /// @param new_state - ����� ��������� �������.
        virtual void set_state( int new_state ) = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� �� ������ ����������� �����.
///
/// �����������, ������ � �.�. �������� ��������� ����� ���������.
class i_AI_device
    {
    public:
        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @return - ������� ��������� ���������� � ���� �������� �����.
        virtual float get_value() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ����������� ������.
///
/// ���������� ����� ���������� � �.�. �������� ��������� ����� ���������.
class i_AO_device: public i_AI_device
    {
    public:
        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;

        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @param new_value - ����� ��������� ����������.
        virtual void set_value( float new_value ) = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ����� �������������� �������� ����������, ������� ������������ � 
/// �������.
class device : public i_simple_device,    
    public i_AO_device,    
    public i_DO_device
    {
    public:
        //-������.
        saved_params_u_int_4 *err_par;

        enum DEV_ERROR_PARAMS  ///< ��������� ������, ���������� ������ �����.
            {
            DE_IS_ENABLE = 1,  ///< ���������� ������� �� ����� ��������������.    
            DE_IS_INHIBIT = 2, ///< ���������� ������� �� ����� ������.    
            DE_IS_SUPPRESS = 4,///< ���������� ������� ���������.
            };

        const char * get_name() const
            {
            return "";
            }
        //-������.!->

        enum CONSTANTS
            {
            C_DEVICE_TYPE_CNT = 14,     ///< ���������� ����� ���������.
            };

        static const char DEV_NAMES[][ 5 ];
        static const char DEV_TYPES[];

        /// ���� ���������.
        enum DEVICE_TYPE
            {
            DT_NONE = -1,      ///< ��� �� ���������.

            DT_V = 0,   ///< ������. 
            DT_N,       ///< �����.
            DT_M,       ///< �������.
            DT_LS,      ///< ������� (����/���).
            DT_TE,      ///< �����������.
            DT_FE,      ///< ������ (��������).
            DT_FS,      ///< ������ (����/���).
            DT_CTR,     ///< �������.
            DT_AO,      ///< ���������� �����.
            DT_LE,      ///< ������� (��������).
            DT_FB,      ///< �������� �����.
            DT_UPR,     ///< ����� ����������.
            DT_QE,      ///< ������������.
            DT_AI,      ///< ���������� ����.
            };

        /// ������� ���������.
        enum DEVICE_SUB_TYPE
            {
            DST_NONE = -1,      ///< ������ �� ���������.

            //DT_V = 0, 
            DST_V_DO_1 = 1,     ///< ������ � ����� ������� ����������.
            DST_V_DO_2,         ///< ������ � ����� �������� ����������.
            DST_V_DO_1_DI_1,    ///< ������ � ����� ������� ���������� � ����� �������� ������.
            DST_V_DO_1_DI_2,    ///< ������ � ����� ������� ���������� � ����� ��������� �������.
            DST_V_DO_2_DI_2,    ///< ������ � ����� �������� ���������� � ����� ��������� �������.
            DST_V_MIXPROOF,     ///< ������ ��������.

            DST_V_1DO_3DI,      ///< ������ � ����� ������� ���������� � ����� ��������� �������.
            DST_V_1DO_2DI_S,    ///< ������ � ����� ������� ���������� � ����� ��������� ������� �� ���� �� ���������.
            DST_V_AS_MIXPROOF,       ///< ������ � ����� �������� ���������� � ����� ��������� ������� � AS ����������� (��������).
            };

        device( int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): number( number ),
            type( type ),
            sub_type( sub_type )            
            { 
            }

        virtual ~device()
            {
            }

        /// @brief ���������� ����������.
        ///
        /// ��������� ���������� � ��������� ���������. ��� ������� ��� ��������
        /// ��� ���������������, �� ���� ���� �� ��������� �������� - ��������.
        virtual void off() = 0;

        /// @brief ���������� ������ ���������� � �����.
        ///
        /// @param buff - ����� ��� ���������� ����������.
        ///
        /// @return -  ���������� ���������� ����.
        int save_device( char *buff );

        /// @brief ����� ������� � �������.
        ///
        /// ��� ������������� � ���������� �����.
        void print() const;

        /// @brief ��������� ������ ����������.
        ///
        /// @return -  ����� ����������.
        u_int_4 get_n() const;

        /// @brief ��������� ���� ����������.
        int get_type() const
            {
            return type;
            }

        /// @brief ��������� ������� ����������.
        int get_sub_type() const
            {
            return sub_type;
            }

    protected:
        u_int_4 number;             ///< ����� ����������.

        DEVICE_TYPE     type;       ///< ��� ����������.
        DEVICE_SUB_TYPE sub_type;   ///< ������ ����������.
    }; 
//-----------------------------------------------------------------------------
/// @brief ����������, ��� �������� ��������� �������� ��������� ���� ����.
///
/// ��� �������, ������, �������, �������� ����� � �.�.
class char_state_device
    {
    public:
        /// @brief ���������� ����������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        int save_changed_state( char *buff );

        /// @brief ���������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///        
        /// @return >= 0 - ���������� ���������� ����.
        int save_state( char *buff  );

        /// @brief ��������� ��������� ����������.
        virtual int get_state() = 0;

    private:
        char prev_state;    ///< ���������� ��������� ����������.
    };
//-----------------------------------------------------------------------------
/// @brief ����������, ��� �������� ��������� �������� ���������� ��� ��������
/// ����� �������� 4 �����.
///
/// �������� �������.
class u_int_4_state_device
    {
    public:
        /// @brief ���������� ����������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_changed_state( char *buff );

        /// @brief ���������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///        
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_state( char *buff );

        /// @brief ��������� ��������� ����������.
        virtual u_int_4 get_u_int_4_state() = 0;

    private:
        u_int_4 prev_state;    ///< ���������� ��������� ����������.
    };
//-----------------------------------------------------------------------------
/// @brief ����������, ��� �������� ��������� �������� ���������� ������� �����
/// �������� 4 �����.
///
/// �������� �����������.
class float_state_device
    {
    public:
        /// @brief ���������� ����������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_changed_state( char *buff );

        /// @brief ���������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///        
        /// @return >= 0 - ���������� ���������� ����.
        virtual int save_state( char *buff );

        /// @brief ��������� ��������� ����������.
        virtual float get_value() = 0;

    private:
        float prev_state;    ///< ���������� ��������� ����������.
    };
//-----------------------------------------------------------------------------
/// @brief ����������� ����������.
///
/// ���������� ��� ����������� ���������� ������ ���������� � ��������������
/// �������. ������ ������� ������ ������ �� ������. 
class dev_stub : public device,
    public i_counter
    {
    public:
        dev_stub(): device( 0, DT_NONE, DST_NONE )
            {
            }

        int     save_state( char *buff );        
        int     save_changed_state( char *buff );
        int     save_device( char *buff );

        u_int_4 get_n() const;                
        void    print() const;                

        float   get_value();
        void    set_value( float new_value );

        void    on();                
        void    off();                
        void    set_state( int new_state );
        int     get_state_now();

        int     parse_cmd( char *buff );

        void    pause();
        void    start();
        void    reset();        
        u_int   get_quantity();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ����� ��� ��������� ���������� ���������.
class digital_device : public device,
    public char_state_device,
    public wago_device
    {
    public:
        digital_device( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): device( number, type, sub_type )
#ifdef DEBUG_NO_WAGO_MODULES
            , state( 0 )
#endif // DEBUG_NO_WAGO_MODULES
            {
            }

        virtual ~digital_device()
            {
            }

        float   get_value();
        void    set_value( float new_value );
        void    set_state( int new_state );  
        int     parse_cmd( char *buff );                        
        void    print() const;

        int save_changed_state( char *buff );
        int save_state( char *buff );                

#ifdef DEBUG_NO_WAGO_MODULES
        /// @brief ��������� ����������� ��������� �������.        
        ///        
        /// @return - ���������� ��������� �������.
        int  get_state_now();

        void on();
        void off();
#endif // DEBUG_NO_WAGO_MODULES

        int get_state();

    protected:
        enum CONSTANTS
            {
            C_SWITCH_TIME = 3, ///< ����� ������������ ����������, ���.
            };

    private:
#ifdef DEBUG_NO_WAGO_MODULES
        char state;  ///< ��������� ����������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ����� ��� ��������� ���������� ���������.
class analog_device : public device,
    public float_state_device,
    public wago_device
    {
    public:
        analog_device( int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): device( number, type, sub_type )
#ifdef DEBUG_NO_WAGO_MODULES
            ,value( 0 )
#endif  // DEBUG_NO_WAGO_MODULES
            {
            }

        void  set_state( int new_state );
        int   get_state_now();
        int   parse_cmd( char *buff );
        
        void  print() const;
        void  on();        
        void  off();

#ifdef DEBUG_NO_WAGO_MODULES
        float get_value();
        void set_value( float new_value );

#else  // DEBUG_NO_WAGO_MODULES

        float get_value() = 0;

#endif // DEBUG_NO_WAGO_MODULES

        int save_changed_state( char *buff );
        int save_state( char *buff );

#ifdef DEBUG_NO_WAGO_MODULES
    private:
        float value;    ///< ��������� ����������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ������, �����, ����� ����������...
class DO_1 : public digital_device
    {
    public:
        DO_1( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� ��������.
///
/// ��� ����� ���� ������, �����...
class DO_2 : public digital_device
    {
    public:
        DO_2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();        

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0, ///< ������ ������ ����������� ������ �1.
            DO_INDEX_2,     ///< ������ ������ ����������� ������ �2.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ���������� ������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_1 : public digital_device
    {
    public:
        DO_1_DI_1( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.
            DI_INDEX = 0,           ///< ������ ������ ����������� �����.

            PAR_FB_STATE = 0,       ///< ������ ��������� ����� �������� �����.
            };

        u_long start_switch_time;  ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_2 : public digital_device
    {
    public:
        DO_1_DI_2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,           ///< ������ ������ ����������� ������.

            DI_INDEX_1 = 0,         ///< ������ �1 ������ ����������� �����.
            DI_INDEX_2,             ///< ������ �2 ������ ����������� �����.
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� �������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_2_DI_2 : public digital_device
    {
    public:
        DO_2_DI_2( int number, device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): digital_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0,         ///< ������ ������ ����������� ������ �1.
            DO_INDEX_2,             ///< ������ ������ ����������� ������ �2.

            DI_INDEX_1 = 0,         ///< ������ �1 ������ ����������� �����.
            DI_INDEX_2,             ///< ������ �2 ������ ����������� �����.
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ mixproof.
class valve_mix_proof : public digital_device
    {
    public:
        valve_mix_proof( u_int number ): digital_device( number, DT_V, DST_V_MIXPROOF )
            {
            }

        enum STATES
            {
            ST_CLOSE = 0,   ///< ������.
            ST_OPEN,        ///< ������.
            ST_UPPER_SEAT,  ///< ������� ������� �����.
            ST_LOW_SEAT,    ///< ������� ������ �����.
            };

        void open_upper_seat();
        void open_low_seat();

#ifndef DEBUG_NO_WAGO_MODULES
        int  get_state_now();
        void on();
        void off();
        void set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            DO_INDEX_U,     ///< ������ ������ ����������� ������ �������� �����.
            DO_INDEX_L,     ///< ������ ������ ����������� ������ ������� �����.

            DI_INDEX_U = 0, ///< ������ ������ ����������� ����� �������� �����.
            DI_INDEX_L,     ///< ������ ������ ����������� ����� ������� �����.
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ AS-mixproof.
class valve_AS_mix_proof : public digital_device
    {
    public:
        valve_AS_mix_proof( u_int number );

        enum STATES
            {
            ST_CLOSE = 0,   ///< ������.
            ST_OPEN,        ///< ������.
            ST_UPPER_SEAT,  ///< ������� ������� �����.
            ST_LOW_SEAT,    ///< ������� ������ �����.
            };

        void open_upper_seat();
        void open_low_seat();

#ifndef DEBUG_NO_WAGO_MODULES
        int  get_state_now();
        void on();
        void off();
        void set_state( int new_state );

    private:
        enum CONSTANTS
            {
            };

        u_long start_switch_time;   ///< ����� ������ ������������ �������.
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �����������, ������ (��������)...
class AI_1 : public analog_device
    {
    public:
        AI_1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): analog_device( number, type, sub_type )
            {
            }
#ifndef DEBUG_NO_WAGO_MODULES
    public:
        float get_value();
        void   set_value( float new_value );

        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ������ ����������.
        virtual float get_min_val() = 0;

    protected:
        enum CONSTANTS
            {
            AI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief �����������.
class temperature_e : public AI_1
    {
    public:
        temperature_e( u_int number ): AI_1( number, DT_TE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������� �������.
class level_e : public AI_1
    {
    public:
        level_e( u_int number ): AI_1( number, DT_LE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������� �����.
class flow_e : public AI_1
    {
    public:
        flow_e( u_int number ): AI_1( number, DT_FE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_MIN_PAR_NUMBER = 0,   ///< ������ ��������� ������������ ��������.
            C_MAX_PAR_NUMBER,       ///< ������ ��������� ������������� ��������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������������.
class concentration_e : public AI_1
    {
    public:
        concentration_e( u_int number ): AI_1( number, DT_QE, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_MIN_PAR_NUMBER = 0,   ///< ������ ��������� ������������ ��������.
            C_MAX_PAR_NUMBER,       ///< ������ ��������� ������������� ��������.
            };                      
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ����������� ������ 4-20 ��.
class analog_input_4_20 : public AI_1
    {
    public:
        analog_input_4_20( u_int number ): AI_1( number, DT_AI, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ����������� ������...
class AO_1 : public analog_device
    {
    public:
        AO_1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type ): analog_device( number, type, sub_type )
            {
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ����� ����������.
        virtual float get_min_val() = 0;

        float get_value();
        void  set_value( float new_value );

    private:
        enum CONSTANTS
            {
            AO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� ����������� ������...
class AO_0_100 : public AO_1
    {
    public:
        AO_0_100( u_int number ) : AO_1( number, DT_AO, DST_NONE )
            {
            }

        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_AO_MIN_VALUE = 0,     ///< ������ ��������� ������������ ��������.
            C_AO_MAX_VALUE = 100,   ///< ������ ��������� ������������� ��������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �������� �����, ������ (����/���)...
class DI_1 : public digital_device
    {
    public:
        DI_1( u_int number, 
            device::DEVICE_TYPE type, 
            device::DEVICE_SUB_TYPE sub_type, u_int dt = 0 ):
        digital_device( number, type, sub_type )
            {
            set_change_time( dt );
            }

#ifndef DEBUG_NO_WAGO_MODULES
    public:
        int  get_state_now();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
#endif // DEBUG_NO_WAGO_MODULES
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ����������.
class valve_DO_1 : public DO_1
    {
    public:
        valve_DO_1( u_int number ) : DO_1( number, DT_V, DST_V_DO_1 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ����������.
class valve_DO_2 : public DO_2
    {
    public:
        valve_DO_2( u_int number ): DO_2( number, DT_V, DST_V_DO_2 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� �������� ������.
class valve_DO_1_DI_1 : public DO_1_DI_1
    {
    public:
        valve_DO_1_DI_1( u_int number ): DO_1_DI_1( number, DT_V, DST_V_DO_1_DI_1 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� ��������� �������.
class valve_DO_1_DI_2 : public DO_1_DI_2
    {
    public:
        valve_DO_1_DI_2( u_int number ): DO_1_DI_2( number, DT_V, DST_V_DO_1_DI_2 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ���������� � ����� ��������� �������.
class valve_DO_2_DI_2 : public DO_2_DI_2
    {
    public:
        valve_DO_2_DI_2( u_int number ): DO_2_DI_2( number, DT_V, DST_V_DO_2_DI_2 )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief �����.
class pump : public DO_1_DI_1
    {
    public:
        pump( u_int number ): DO_1_DI_1( number, DT_N, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief �������.
class mixer : public DO_1_DI_1
    {
    public:
        mixer( u_int number ): DO_1_DI_1( number, DT_M, DST_NONE )
            {
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� ������.
class level_s : public DI_1
    {
    public:
        level_s( u_int number, u_int dt = 1000 ): DI_1( number, DT_LS, DST_NONE )
            {
            set_change_time( dt );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� �������.
class flow_s : public DI_1
    {
    public:
        flow_s( u_int number ): DI_1( number, DT_FS, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ �������� �����.
class feedback : public DI_1
    {
    public:
        feedback( u_int number ): DI_1( number, DT_FB, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ ����������
class control_s : public DO_1
    {
    public:
        control_s( u_int number ): DO_1( number, DT_UPR, DST_NONE )
            {            
            }
    };
//-----------------------------------------------------------------------------
/// @brief �������.
class counter : public device,
    public i_counter,
    public u_int_4_state_device,
    public wago_device
    {
    public:
        counter( u_int number ): device( number, DT_CTR, DST_NONE ), 
            value( 0 ),
            last_read_value( 0 )
            {            
            }

        virtual ~counter()
            {            
            }

        float get_value();
        void  set_value( float new_value );
        int   get_state_now();
        void  on();
        void  off();
        void  set_state( int new_state );
        int   parse_cmd( char *buff  );
        void  print() const;

        u_int_4 get_u_int_4_state();

        int save_changed_state( char *buff );
        int save_state( char *buff );

        void  pause();
        void  start();
        void  reset();
        u_int get_quantity();

    private:
        enum STATES
            {
            S_STOP,
            S_WORK,
            S_PAUSE,
            };

        enum CONSTANTS
            {
            AI_INDEX = 0,       ///< ������ ������ ����������� �����.

            MAX_VAL = 65535,    ///< ������������ �������� ��������.
            };

        u_int value;
        u_int last_read_value;

        STATES state;
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������.
///
/// �������� ���������� ��� ���� ����������� �������.
class device_manager
    {
    public:
        device_manager();

        ~device_manager();

        /// @brief ��������� ���������� �� ��� ������.        
        device* get_device( device::DEVICE_TYPE dev_type, u_int dev_number );

        /// @brief ��������� ������� �� ��� ������.
        i_DO_device* get_V( int number );

        /// @brief ��������� ������ �� ������.
        i_DO_device* get_N( int number );

        /// @brief ��������� ������� �� ������.
        i_DO_device* get_M( int number );

        /// @brief ��������� ������ �� ������.
        i_DI_device* get_LS( int number );

        /// @brief ��������� ������� �� ������.
        i_DI_device* get_FS( int number );

        /// @brief ��������� ����������� ����� �� ������.
        i_AI_device* get_AI( int number );

        /// @brief ��������� ����������� ������ �� ������.
        i_AO_device* get_AO( int number );

        /// @brief ��������� �������� �� ������.
        i_counter* get_CTR( int number );

        /// @brief ��������� ����������� �� ������.
        i_AI_device* get_TE( int number );

        /// @brief ��������� �������� ������� �� ������.
        i_AI_device* get_FE( int number );

        /// @brief ��������� �������� ������ �� ������.
        i_AI_device* get_LE( int number );

        /// @brief ��������� �������� ����� �� ������.
        i_DI_device* get_FB( int number );

        /// @brief ��������� ������������ ������ �� ������.
        i_DO_device* get_UPR( int number );

        /// @brief ��������� ������� ������������ �� ������.
        i_AI_device* get_QE( int number );

        /// @brief ��������� ������������� ���������� ������.
        static device_manager* get_instance();

        /// @brief ���������� ������ ������� � �������.
        void print() const;

        /// @brief ��������� ������������� ���������� ������.
        static void set_instance( device_manager* new_instance );

        /// @brief ��������� ���� ������� ���������, ��� �������� �� ������.
        complex_device * get_communication_device()
            {
            return devices;
            }

        /// @brief ��������� ���������� ����������.
        dev_stub* get_stub()
            {
            return &stub;
            }

    protected:
        /// ��� ������� ����������, ��� �������� �� ������.
        complex_device *devices;

        dev_stub stub;  ///< ����������-��������, ��������� ����������. 

        struct range    ///< �������� ��������� ������ ����. 
            {
            int start_pos;
            int end_pos;
            };

        /// ��������� ��������� ���� �����.
        range dev_types_ranges[ device::C_DEVICE_TYPE_CNT ];
        
        /// @brief ��������� ������� ���������� �� ��� ������.        
        int get_device_n( device::DEVICE_TYPE dev_type, u_int dev_number );

        std::vector< device* > project_devices; ///< ��� ����������.

        /// @brief ������������ ��������� ������.
        static auto_smart_ptr < device_manager > instance;

    public:
        // @brief ��������� ����� ���������.
        //
        // ���������� �� Lua.
        wago_device* add_device( int dev_type, int dev_sub_type, 
            u_int number, char * descr );

        void complete_init()
            {
            devices = new complex_device( 0, "GLB", device::C_DEVICE_TYPE_CNT, 0 );

            for ( int i = 0; i < device::C_DEVICE_TYPE_CNT; i++ )
                {
                int dev_cnt = dev_types_ranges[ i ].end_pos -
                    dev_types_ranges[ i ].start_pos + 1;

                if ( dev_types_ranges[ i ].start_pos == -1 )
                    {
                    dev_cnt = 0;
                    }

                devices->sub_dev[ i ] =
                    new complex_device( 0, device::DEV_NAMES[ i ], dev_cnt,
                    device::DEV_TYPES[ i ] );

                if ( dev_cnt )
                    {
                    int pos = 0;
                    for ( int j = dev_types_ranges[ i ].start_pos;
                        j <= dev_types_ranges[ i ].end_pos; j++ )
                        {
                        ( ( complex_device* ) ( devices->sub_dev[ i ] ) )->sub_dev[ pos++ ] =
                            project_devices[ j ];
                        }
                    }
                }
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������.
///
/// ������������� ���������������� �������.
class timer
    {
    public:
        enum STATE
            {
            S_STOP = 0, ///< �� ��������.
            S_WORK,     ///< ��������.
            S_PAUSE,    ///< �����.
            };

        /// @brief ���������� ��������� ���������� � �����.
        ///
        /// @param buff [ out ] - ����� ������, ���� ����� ������������ ������.
        ///        
        /// @return >= 0 - ���������� ���������� ����.
        int save( char *buff );

        /// @brief ���������� ��������� ���������� �� ������.
        ///
        /// @param buff [ out ] - ����� ������, ������ ����� ����������� ������.
        ///        
        /// @return >= 0 - ���������� ��������� ����.
        int load( char *buff );

        /// @brief ��������� ������� ������ ��� ���������� ��������� ����������.
        ///
        /// @return >= 0 - ������ ������, ����.
        int get_saved_size() const;

        timer();

        /// @brief ������ �������.
        void start();

        /// @brief ����� �������.
        void reset();

        /// @brief ����� �������.
        void pause();

        /// @brief �������� ������ ������� �������.
        ///
        /// @return 1 - ����� �����.
        /// @return 0 - ����� �� �����.
        bool is_time_up() const;

        /// @brief ��������� ������� ������ �������.
        ///
        /// @return - ����� ������ �������.
        u_long  get_work_time() const;

        /// @brief ��������� ������� �������.
        ///
        /// @param new_countdown_time - �������.
        void set_countdown_time( u_long new_countdown_time );

        /// @brief ��������� ������� �������.
        ///
        /// @return - ������� �������.
        u_long  get_countdown_time() const;

        /// @brief ��������� ��������� �������.
        ///
        /// @return - ��������� �������.
        STATE get_state() const;

    private:
        u_long  last_time;  ///< �����, ����� ������ ��� �������/����������.
        u_long  work_time;  ///< ����� ������ �������.

        STATE   state;           ///< ���������.
        u_long  countdown_time;  ///< ������� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������.
///
/// ���������� ������ ��������.
class timer_manager
    {
    public:
        /// @param timers_count - ���������� �������� � ������.
        timer_manager( u_int timers_count );

        ~timer_manager();

        /// @brief ���������� ��������� ������� �� �������.
        ///
        /// @param index - ������ �������.
        ///
        /// @return - ������ � ������ ��������, �������� - � ������ ������ �� 
        /// ��������.
        timer& operator[] ( unsigned int index );

    private:
        u_int   timers_cnt; ///< ���������� ��������.
        timer   *timers;    ///< �������.

        timer   stub;       ///< ��������.
    };
//-----------------------------------------------------------------------------
///@brief ��������� ��������� ���������.
///
///@return �������� ��������� �������.
device_manager* G_DEVICE_MANAGER();
//-----------------------------------------------------------------------------
/// @brief ��������� ������� �� ������.
///
/// @param number - ����� �������.
/// @return - ������ � �������� �������. ���� ��� ������ �������, ������������
/// �������� (@ref dev_stub).
i_DO_device* V( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ �� ������.
///
/// @param number - ����� ������.
/// @return - ����� � �������� �������. ���� ��� ������ ������, ������������
/// �������� (@ref dev_stub).
i_DO_device* N( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� �� ������.
///
/// @param number - ����� �������.
/// @return - ������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* M( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������ �� ������.
///
/// @param number - ����� ����������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* LS( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������� �� ������.
///
/// @param number - ����� ����������� �������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* FS( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ����� �� ������.
///
/// @param number - ����� ����������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* AI( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� ������ �� ������.
///
/// @param number - ����� ����������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AO_device* AO( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� �� ������.
///
/// @param number - ����� ��������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_counter* CTR( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ����������� �� ������.
///
/// @param number - ����� �����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* TE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������� �� ������.
///
/// @param number - ����� �������� �������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* FE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������ �� ������.
///
/// @param number - ����� �������� ������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* LE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ����� �� ������.
///
/// @param number - ����� �������� �����.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DI_device* FB( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ ���������� �� ������.
///
/// @param number - ����� ������ ����������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_DO_device* UPR( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ������������ �� ������.
///
/// @param number - ����� ������� ������������.
/// @return - ���������� � �������� �������. ���� ��� ������ ����������,
/// ������������ �������� (@ref dev_stub).
i_AI_device* QE( int number );
//-----------------------------------------------------------------------------
/// @brief ��������� ������ �� ������.
///
/// @return - ����������� ����������.
dev_stub* STUB();
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
