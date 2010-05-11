/// @file PAC_dev.h
/// @brief �������� �������� �������, ������� ��������� ������� �������� 
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
        /// @return - �������� �������� � �������� @ref idx.
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

        /// ���� ���������.
        enum DEVICE_TYPE
            {
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
            DST_NONE = 0,       ///< ������ �����������.

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
        int save_device( char *buff  );

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

        int get_type() const
            {
            return type;
            }

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
        /// @brief ���������� ���������� @ref i_save_device.
        int save_changed_state( char *buff );

        /// @brief ���������� ���������� @ref i_save_device.
        int save_state( char *buff  );

        /// @brief .
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
        /// @brief ���������� ���������� @ref i_save_device.
        virtual int save_changed_state( char *buff );

        /// @brief ���������� ���������� @ref i_save_device.
        virtual int save_state( char *buff );

        /// @brief ��� ���������� ��������� ����������.                
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
        /// @brief ���������� ���������� @ref i_save_device.
        virtual int save_changed_state( char *buff );

        /// @brief ���������� ���������� @ref i_save_device.
        virtual int save_state( char *buff );

        /// @brief .
        virtual float get_value() = 0;

    private:
        float prev_state;    ///< ���������� ��������� ����������.
    };
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ������� �����/������ WAGO. 
/// 
/// � ����� ������ � ���������� ����� ���� ���� ��� ��������� �������
/// �����/������ (���������� ��� ����������).
class wago_device
    {
    public:
        static char debug_mode;

        wago_device();

        /// @brief �������� ������ ���������� �� ������.
        ///
        /// @param cfg_file - ���������� ��������� ���������� ����� � ���������
        ///   ����������.
        ///
        /// @return -  ���������� ��������� ����.
        virtual int load( file *cfg_file );

    protected:
        enum CONSTANTS
            {
            C_MAX_ANALOG_CHANNEL_VALUE = 65535,
            };

        /// @brief ��������� ��������� ������ ����������� ������.
        ///
        /// @param index - ������ ������ � ������� ���������� �������� ������� 
        /// ����������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int get_DO( u_int index );

        /// @brief ��������� ��������� ������ ����������� ������.
        ///
        /// @param index - ������ ������ � ������� ���������� �������� ������� 
        /// ����������.
        /// @param value - ����� ��������� ������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int set_DO( u_int index, char value );

        /// @brief ��������� ��������� ������ ����������� �����.
        ///
        /// @param index - ������ ������ � ������� ���������� ������� ������� 
        /// ����������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int get_DI( u_int index );

        /// @brief ��������� ��������� ������ ����������� �����.
        ///
        /// @param index - ������ ������ � ������� ���������� ������� ������� 
        /// ����������.
        /// @param value - ����� ��������� ������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int set_DI( u_int index, char value );

        /// @brief ��������� ��������� ������ ����������� ������.
        ///
        /// @param index - ������ ������ � ������� ���������� �������� ������� 
        /// ����������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        u_int get_AO( u_int index );

        /// @brief ��������� ��������� ������ ����������� ������.
        ///
        /// @param index - ������ ������ � ������� ���������� �������� ������� 
        /// ����������.
        /// @param value - ����� ��������� ������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int set_AO( u_int index, u_int value );

        /// @brief ��������� ��������� ������ ����������� �����.
        ///
        /// @param index - ������ ������ � ������� ���������� ������� ������� 
        /// ����������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        u_int get_AI( u_int index );

        /// @brief ��������� ��������� ������ ����������� �����.
        ///
        /// @param index - ������ ������ � ������� ���������� ������� ������� 
        /// ����������.
        /// @param value - ����� ��������� ������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int   set_AI( u_int index, u_int value );

        float get_par( u_int index );

        virtual void print() const;

    private:
        /// @brief ������ ������� �����/������ ����������.
        struct IO_channels
            {
            enum CHANNEL_TYPE
                {
                CT_DI = 1,
                CT_DO,
                CT_AI,
                CT_AO,
                };

            u_int  count;   ///< ���������� �������.
            u_int* tables;  ///< ������ ������.
            u_int* offsets; ///< ������ �������� � �������� ������.

            u_int  **int_read_values;    ///< ������ �������� ��� ������.
            u_int  **int_write_values;   ///< ������ �������� ��� ������.
            u_char **char_read_values;   ///< ������ �������� ��� ������.
            u_char **char_write_values;  ///< ������ �������� ��� ������.

            IO_channels( CHANNEL_TYPE type ) : count( 0 ), tables( 0 ),
                offsets( 0 ), 
                int_read_values( 0 ), int_write_values( 0 ),
                char_read_values( 0 ), char_write_values( 0 ),
                type( type )
                {
                }

            CHANNEL_TYPE type;
            };

        IO_channels DI_channels;    ///< ������ ����������� �����.
        IO_channels DO_channels;    ///< ������ ����������� ������.
        IO_channels AI_channels;    ///< ������ ����������� �����.
        IO_channels AO_channels;    ///< ������ ����������� ������.

        u_int   params_count;
        float   *params;

        void print_table( const char *str, const IO_channels &channels ) const;

        /// @brief �������� ���������� � ������ ������� �����/������ �� 
        /// ��������� ������.
        /// 
        /// @param stream - ����� ��� ���������� ����������.
        /// @param [out] channels - ������, � ������� �����������.
        ///
        /// @return -  ���������� ��������� ����.
        int load_table_from_string( char *str, IO_channels &channels );
    };
//-----------------------------------------------------------------------------
/// @brief ����������� ����������.
///
/// ���������� ��� ����������� ���������� ������ ���������� � ��������������
/// �������. ������ ������� ������ ������ �� ������. 
class dev_stub : public device
    {
    public:
        int save_state( char *buff )
            {
            return 0;
            }
        int save_changed_state( char *buff )
            {
            return 0;
            }
        int save_device( char *buff )
            {
            return 0;
            }
        u_int_4 get_n() const
            {
            return 0;
            }
        void print() const
            {
#ifdef DEBUG
            Print( "virtual device" );
#endif // DEBUG
            }

        float get_value();

        int set_value( float new_value );

        int get_state();

        void on();

        void off();

        int set_state( int new_state );

        int parse_cmd( char *buff );

        int load( file *cfg_file );
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ���� ��� ��������� ���������� ���������.
class digital_device : public device,
    public char_state_device,
    public wago_device
    {
    public:
        float get_value();

        int set_value( float new_value );

        int set_state( int new_state );

        int parse_cmd( char *buff );

        int load( file *cfg_file );

        void print() const;

        /// @brief ���������� ���������� @ref i_save_device.
        int save_changed_state( char *buff )
            {
            return char_state_device::save_changed_state( buff );
            }

        /// @brief ���������� ���������� @ref i_save_device.
        int save_state( char *buff )
            {
            return char_state_device::save_state( buff );
            }

        /// @brief .
        int get_state() = 0;

    protected:
        enum CONSTANTS
            {
            C_SWITCH_TIME = 5000,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����������� �������/��������.
///
/// ������� ���� ��� ��������� ���������� ���������.
class analog_device : public device,
    public float_state_device,
    public wago_device
    {
    public:
        float get_value() = 0;

        int set_state( int new_state )
            {
            return set_value( new_state );
            }

        int get_state()
            {
            return ( int ) get_value();
            }

        int parse_cmd( char *buff )
            {
            set_value( *( ( float* ) buff ) );
            return sizeof( float );
            }

        int load( file *cfg_file )
            {
            device::load( cfg_file );
            wago_device::load( cfg_file );

            return 0;
            }

        void print() const
            {
            device::print();
            wago_device::print();
            }

        void on()
            {
            }

        void off()
            {
            set_value( 0 );
            }

        /// @brief ���������� ���������� @ref i_save_device.
        int save_changed_state( char *buff )
            {
            return float_state_device::save_changed_state( buff );
            }

        /// @brief ���������� ���������� @ref i_save_device.
        int save_state( char *buff )
            {
            return float_state_device::save_state( buff );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� �������.
///
/// ��� ����� ���� ������, �����, ����� ����������...
class DO_1 : public digital_device
    {
    public:
        int get_state();

        void on();

        void off();

        int set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� ��������.
///
/// ��� ����� ���� ������, �����...
class DO_2 : public digital_device
    {
    public:
        int get_state();

        void on();

        void off();

        int set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0,
            DO_INDEX_2,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ���������� ������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_1 : public digital_device
    {
    public:

        int get_state();

        void on();

        void off();

        int set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,
            DI_INDEX = 0,

            PAR_FB_STATE = 0,
            };

        u_long switch_time;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_1_DI_2 : public digital_device
    {
    public:
        int get_state();

        void on();

        void off();

        int set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,

            DI_INDEX_1 = 0,
            DI_INDEX_2,
            };

        u_long switch_time;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ����������� �������� � ����� ����������� �������.
///
/// ��� ����� ���� ������, �����...
class DO_2_DI_2 : public digital_device
    {
    public:
        int get_state();

        void on();

        void off();

        int set_state( int new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX_1 = 0,
            DO_INDEX_2,

            DI_INDEX_1 = 0,
            DI_INDEX_2,
            };

        u_long switch_time;
    };
//-----------------------------------------------------------------------------
/// @brief ������ mixproof.
class mix_proof : public digital_device
    {
    public:

        int get_state();

        void on();

        void open_upper_seat();

        void open_low_seat();

        void off();

        enum STATES
            {
            ST_CLOSE = 0,
            ST_OPEN,
            ST_UPPER_SEAT,
            ST_LOW_SEAT,
            };

        int set_state( STATES new_state );

    private:
        enum CONSTANTS
            {
            DO_INDEX = 0,
            DO_INDEX_U,
            DO_INDEX_L,

            DI_INDEX_U = 0,
            DI_INDEX_L,
            };

        u_long switch_time;
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �����������, ������ (��������)...
class AI_1 : public analog_device
    {
    public:
        float get_value();

        int set_value( float new_value );

        virtual float get_max_val() = 0;
        virtual float get_min_val() = 0;

    protected:
        enum CONSTANTS
            {
            AI_INDEX = 0,
            };
    };
//---------------------------------l--------------------------------------------
/// @brief ���������� ...
class temperature_e : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();

        float get_value();

        int set_value( float new_value );
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ...
class level_e : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ...
class flow_e : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_MIN_PAR_NUMBER = 0,
            C_MAX_PAR_NUMBER,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ...
class concentration_e : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();

    private:
        enum CONSTANTS
            {
            C_MIN_PAR_NUMBER = 0,
            C_MAX_PAR_NUMBER,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ...
class analog_input_4_20 : public AI_1
    {
    public:
        float get_max_val();
        float get_min_val();
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� ����������� ������...
class AO_1 : public analog_device
    {
    public:
        float get_value();

        int set_value( float new_value );

    private:
        enum CONSTANTS
            {
            AO_INDEX = 0,

            C_AO_MIN_VALUE = 0,
            C_AO_MAX_VALUE = 100,
            };
    };
//-----------------------------------------------------------------------------
/// @brief ���������� � ����� ���������� ������.
///
/// ��� ����� ���� �������� �����, ������ (����/���)...
class DI_1 : public digital_device
    {
    public:
        int get_state();

        void on();

        void off();

    private:
        enum CONSTANTS
            {
            DI_INDEX = 0,
            };
    };
//-----------------------------------------------------------------------------
/// @brief �������.
///
class counter : public device,
    public u_int_4_state_device,
    public wago_device
    {
    public:        
        float get_value();

        int set_value( float new_value );

        int get_state();

        void on();

        void off();

        int set_state( int new_state );

        int parse_cmd( char *buff  );

        int load( file *cfg_file );

        void print() const;

        u_int_4 get_u_int_4_state();

        /// @brief ���������� ���������� @ref i_save_device.
        int save_changed_state( char *buff )
            {
            return u_int_4_state_device::save_changed_state( buff );
            }

        /// @brief ���������� ���������� @ref i_save_device.
        int save_state( char *buff )
            {
            return u_int_4_state_device::save_state( buff );
            }

    private:
        enum CONSTANTS
            {
            AI_INDEX = 0,
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

        int load_from_cfg_file( file *cfg_file );

        i_DO_device* get_V( int number );

        i_DO_device* get_N( int number );

        i_DO_device* get_M( int number );

        i_DI_device* get_LS( int number );

        i_DI_device* get_FS( int number );

        i_AI_device* get_AI( int number );

        i_AO_device* get_AO( int number );

        counter* get_CTR( int number );

        i_AI_device* get_TE( int number );

        i_AI_device* get_FE( int number );

        i_AI_device* get_LE( int number );

        i_DI_device* get_FB( int number );

        i_DO_device* get_UPR( int number );

        i_AI_device* get_QE( int number );

        static device_manager* get_instance();

        void print() const;

        static void set_instance( device_manager* new_instance );

    private:
        dev_stub stub;

        struct range
            {
            int start_pos;
            int end_pos;
            };

        range dev_types_ranges[ device::C_DEVICE_TYPE_CNT ];

        /// @brief ��������� ���������� �� ��� ������.
        ///
        device* get_device( device::DEVICE_TYPE dev_type, u_int dev_number,
            char const * dev_name );

        /// @brief ��������� ��������� ���������� �� ��� ������.
        ///
        int get_device_n( device::DEVICE_TYPE dev_type, u_int dev_number );

        int    devices_count;
        device **project_devices;

        static device_manager* instance;
    };
//-----------------------------------------------------------------------------
// ��������� ���������������� ���������� �� ��� ������.
#define V   device_manager::get_instance()->get_V
#define N   device_manager::get_instance()->get_N
#define M   device_manager::get_instance()->get_M
#define LS  device_manager::get_instance()->get_LS
#define FS  device_manager::get_instance()->get_FS
#define AI  device_manager::get_instance()->get_AI
#define AO  device_manager::get_instance()->get_AO
#define CTR device_manager::get_instance()->get_CTR
#define TE  device_manager::get_instance()->get_TE
#define FE  device_manager::get_instance()->get_FE
#define LE  device_manager::get_instance()->get_LE
#define FB  device_manager::get_instance()->get_FB
#define UPR device_manager::get_instance()->get_UPR
#define QE  device_manager::get_instance()->get_QE
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
