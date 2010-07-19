/// @file PAC_dev.h
/// @brief ������, ������� ��������� ������� �������� 
/// ��������� ��������� PAC �� ������. ����� ����������� ������ ��� ������ � 
/// ������������ ( �������, ������,... ). ������ ������������ ������ � 
/// ����������� ( PAC ).
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

#ifndef PAC_DEVICES_H
#define PAC_DEVICES_H

#include <string.h>
#include <stdio.h>

#include <string>

#include "sys.h"
#include "wago.h"

#include "g_device.h"

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
            ( ( u_int_4* ) ( buff + idx ) )[ 0 ] = n;           //(2)
            idx += 4;
            buff[ idx++ ] = strlen( name );                     //(3)
            strcpy( buff + idx, name );                         //(4)
            idx += strlen( name ) + 1;
            ( ( u_int_4* ) ( buff + idx ) )[ 0 ] = sub_dev_cnt; //(5)
            idx += 4;           

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
            ( ( u_int_4* ) buff )[ 0 ] = n;                     //(1)
            ( ( u_int_4* ) buff )[ 1 ] = sub_dev_cnt;           //(2)
            u_int_2 answer_size = 8;

            for ( u_int_4 i = 0; i < sub_dev_cnt; i++ )
                {
                ( ( data_type* ) ( buff + 8 ) )[ 0 ] = 
                    get_val( i );
                buff += sizeof( data_type );
                prev_val[ i ] = get_val( i );
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
            T_POST,
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

        u_int_4 get_val( int idx );
        int     parse_cmd( char *buff  );
        //void    print() const;
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
/// @brief ��������� ��������.
class i_counter
    {
    public:
        /// @brief ������������ ������ ��������.
        virtual void pause() = 0;

        /// @brief ������������� ������ ��������.
        virtual void start() = 0;

        /// @brief ����� ��������.
        /// 
        /// ����� ������ ��� ����������� ������ ���������� ������� @ref start().
        virtual void reset() = 0;

        /// @brief ��������� �������� ��������.
        virtual u_int get_quantity() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ���������� ������.
///
/// ���������� ���� �������� �����, ������ � �.�. ��������� ������ ���������.
class i_DI_device
    {
    public:
        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @return - ������� ��������� ���������� � ���� ������ �����.
        virtual int get_state() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ���������� �������.
///
/// ���������� ���� �������, ������� � �.�. ��������� ������ ���������.
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
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        virtual int set_state( int new_state ) = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ���������� ������.
///
/// ���������� ���� �����������, ������� � �.�. ��������� ������ ���������.
class i_AI_device
    {
    public:
        /// @brief ��������� �������� ��������� ����������.
        ///
        /// @return - ������� ��������� ���������� � ���� �������� �����.
        virtual float get_value() = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ���������� �������.
///
/// ���������� ���� ����������� ������ ���������� � �.�. ��������� ������
/// ���������.
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
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        virtual int set_value( float new_value ) = 0;
    };
//-----------------------------------------------------------------------------
/// @brief ����� �������������� �������� ����������, ������� ������������ � 
/// �������.
class device : public i_simple_device,    
    public i_AO_device,    
    public i_DO_device
    {
    public:
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
            };

        device();

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

        /// @brief �������� ������ ���������� �� ������.
        ///
        /// @param cfg_file - ���������� ��������� ���������� ����� � ���������
        ///   ����������.
        ///
        /// @return - 0 - ��.
        virtual int load( file *cfg_file );

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
        int     save_state( char *buff );        
        int     save_changed_state( char *buff );
        int     save_device( char *buff );

        u_int_4 get_n() const;                
        void    print() const;                

        float   get_value();
        int     set_value( float new_value );                
                        
        void    on();                
        void    off();                
        int     set_state( int new_state );
        int     get_state();
                
        int     parse_cmd( char *buff );
               
        int     load( file *cfg_file ); 

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
        float   get_value();
        int     set_value( float new_value );
        int     set_state( int new_state );  
        int     parse_cmd( char *buff );                     
        int     load( file *cfg_file );      
        void    print() const;

        int save_changed_state( char *buff );
        int save_state( char *buff );        
        int get_state() = 0;

    protected:
        enum CONSTANTS
            {
            C_SWITCH_TIME = 5, ///< ����� ������������ �������, ���.
            };
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
        float   get_value() = 0;
        int     set_state( int new_state );
        int     get_state();
        int     parse_cmd( char *buff );
        int     load( file *cfg_file );
        void    print() const;
        void    on();        
        void    off();

        int save_changed_state( char *buff );
        int save_state( char *buff );
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ������, �����, ����� ����������...
class DO_1 : public digital_device
    {
    public:
        int  get_state();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� ��������.
///
/// ��� ����� ���� ������, �����...
class DO_2 : public digital_device
    {
    public:
        int  get_state();
        void on();
        void off();        

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0, ///< ������ ������ ����������� ������ �1.
            DO_INDEX_2,     ///< ������ ������ ����������� ������ �2.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ���������� ������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_1 : public digital_device
    {
    public:
        int  get_state();
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
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_2 : public digital_device
    {
    public:
        int  get_state();
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
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� �������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_2_DI_2 : public digital_device
    {
    public:
        int  get_state();
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
    };
//-----------------------------------------------------------------------------
/// @brief ������ mixproof.
class valve_mix_proof : public digital_device
    {
    public:
        int  get_state();
        void on();
        void open_upper_seat();
        void open_low_seat();
        void off();

        enum STATES
            {
            ST_CLOSE = 0,   ///< ������.
            ST_OPEN,        ///< ������.
            ST_UPPER_SEAT,  ///< ������� ������� �����.
            ST_LOW_SEAT,    ///< ������� ������ �����.
            };

        int set_state( int new_state );

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
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �����������, ������ (��������)...
class AI_1 : public analog_device
    {
    public:
        float get_value();
        int   set_value( float new_value );

        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ������ ����������.
        virtual float get_min_val() = 0;

    protected:
        enum CONSTANTS
            {
            AI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
    };
//---------------------------------l--------------------------------------------
/// @brief �����������.
class temperature_e : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();

        float get_value();
        int   set_value( float new_value );
    };
//-----------------------------------------------------------------------------
/// @brief ������� �������.
class level_e : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ������� �����.
class flow_e : public AI_1
    {
    public:
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
        /// @brief ��������� ������������� �������� ������ ����������.
        virtual float get_max_val() = 0;

        /// @brief ��������� ������������ �������� ����� ����������.
        virtual float get_min_val() = 0;

        float get_value();
        int   set_value( float new_value );

    private:
        enum CONSTANTS
            {
            AO_INDEX = 0,   ///< ������ ������ ����������� ������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� ����������� ������...
class AO_0_100 : public AO_1
    {
    public:
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
        int  get_state();
        void on();
        void off();

    private:
        enum CONSTANTS
            {
            DI_INDEX = 0,   ///< ������ ������ ����������� �����.
            };
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ����������.
class valve_DO_1 : public DO_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ����������.
class valve_DO_2 : public DO_2
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� �������� ������.
class valve_DO_1_DI_1 : public DO_1_DI_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� ������� ���������� � ����� ��������� �������.
class valve_DO_1_DI_2 : public DO_1_DI_2
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����� �������� ���������� � ����� ��������� �������.
class valve_DO_2_DI_2 : public DO_2_DI_2
    {
    };
//-----------------------------------------------------------------------------
/// @brief �����.
class pump : public DO_1_DI_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief �������.
class mixer : public DO_1_DI_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� ������.
class level_s : public DI_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ ������������� �������.
class flow_s : public DI_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ �������� �����.
class feedback : public DI_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief ������ ����������
class control_s : public DO_1
    {
    };
//-----------------------------------------------------------------------------
/// @brief �������.
class counter : public device,
    public i_counter,
    public u_int_4_state_device,
    public wago_device
    {
    public:        
        float get_value();
        int   set_value( float new_value );
        int   get_state();
        void  on();
        void  off();
        int   set_state( int new_state );
        int   parse_cmd( char *buff  );
        int   load( file *cfg_file );
        void  print() const;

        u_int_4 get_u_int_4_state();

        int save_changed_state( char *buff );
        int save_state( char *buff );

        void  pause();
        void  start();
        void  reset();
        u_int get_quantity();

    private:
        enum CONSTANTS
            {
            AI_INDEX = 0, ///< ������ ������ ����������� �����.
            };
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������.
///
/// �������� ���������� ��� ���� ����������� �������.
class device_manager
    {
    public:
        device_manager();

        /// @brief �������� �� ����� ������������.
        int load_from_cfg_file( file *cfg_file );

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

    public:
        dev_stub stub;  ///< ����������-��������, ��������� ����������. 

        struct range    ///< �������� ��������� ������ ����. 
            {
            int start_pos;
            int end_pos;
            };

        /// ��������� ��������� ���� �����.
        range dev_types_ranges[ device::C_DEVICE_TYPE_CNT ];

        /// @brief ��������� ���������� �� ��� ������.        
        device* get_device( device::DEVICE_TYPE dev_type, u_int dev_number,
            char const * dev_name );

        /// @brief ��������� ������� ���������� �� ��� ������.        
        int get_device_n( device::DEVICE_TYPE dev_type, u_int dev_number );

        int    devices_count;               ///< ���������� ���������.
        device **project_devices;           ///< ��� ����������.

        static device_manager* instance;    ///< ������������ ��������� ������.
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

        int save( char *buff );

        int load( char *buff );

        int get_saved_size() const;

        timer();

        void    start();

        void    reset();

        void    pause();

        bool    is_time_up() const;

        u_long  get_work_time() const;

        void    set_countdown_time( u_long new_countdown_time );

        u_long  get_countdown_time() const;

        STATE   get_state() const;

    private:
        u_long  last_time;  ///< �����, ����� ������ ��� �������/����������.
        u_long  work_time;  ///< ����� ������ �������.

        STATE   state;           ///< ���������.
        u_long  countdown_time;  ///< ������� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������.
///
/// ������������� ������ ��������.
class timer_manager
    {
    public:
        timer_manager( u_int timers_count ): timers_cnt( timers_count ),
            timers( 0 )
        {
        if ( timers_cnt )
            {
            timers = new timer[ timers_cnt ];
            }   
        }

        ~timer_manager()
            {
            if ( timers )
                {
                delete [] timers;
                timers = 0;
                }
            }

    timer& operator[] ( unsigned int index )
        {
        if ( index < timers_cnt )
                {
                return timers[ index ];
                }
#ifdef DEBUG
            else
                {
                Print( "timer_manager[] - error: index[ %u ] > count [ %u ]\n",
                    index, timers_cnt );
                }
#endif // DEBUG

            return stub;
        }

    private:
        u_int   timers_cnt;
        timer   *timers;

        timer   stub;
    };
//-----------------------------------------------------------------------------
#define G_DEVICE_MANAGER device_manager::get_instance()
//-----------------------------------------------------------------------------
#define V   G_DEVICE_MANAGER->get_V   ///< ��������� ������� �� ������.
#define N   G_DEVICE_MANAGER->get_N   ///< ��������� ������ �� ������.
#define M   G_DEVICE_MANAGER->get_M   ///< ��������� ������� �� ������.
#define LS  G_DEVICE_MANAGER->get_LS  ///< ��������� ����������� ������ �� ������.
#define FS  G_DEVICE_MANAGER->get_FS  ///< ��������� ����������� ������� �� ������.
#define AI  G_DEVICE_MANAGER->get_AI  ///< ��������� ����������� ����� �� ������.
#define AO  G_DEVICE_MANAGER->get_AO  ///< ��������� ����������� ������ �� ������.
#define CTR G_DEVICE_MANAGER->get_CTR ///< ��������� �������� �� ������.
#define TE  G_DEVICE_MANAGER->get_TE  ///< ��������� ����������� �� ������.
#define FE  G_DEVICE_MANAGER->get_FE  ///< ��������� �������� ������� �� ������.
#define LE  G_DEVICE_MANAGER->get_LE  ///< ��������� �������� ������ �� ������.
#define FB  G_DEVICE_MANAGER->get_FB  ///< ��������� �������� ����� �� ������.
#define UPR G_DEVICE_MANAGER->get_UPR ///< ��������� ������ ���������� �� ������.
#define QE  G_DEVICE_MANAGER->get_QE  ///< ��������� ������� ������������ �� ������.
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
