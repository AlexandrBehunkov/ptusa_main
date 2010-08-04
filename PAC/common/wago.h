/// @file wago.h
/// @brief ������ � Wago - ���������� �� ������ ������ � ������� Wago, ����
/// Wago.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.
/// 
/// @par ������� ������:
/// @$Rev: 79 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-05-14 09:54:41#$.

#ifndef WAGO_H
#define WAGO_H

#include "sys.h"
//-----------------------------------------------------------------------------
/// @brief ���������� �� ������ ������� �����/������ WAGO. 
/// 
/// � ����� ������ � ���������� ����� ���� ���� ��� ��������� �������
/// �����/������ (���������� ��� ����������).
class wago_device
    {
    public:
        wago_device();

        /// @brief �������� ������ ���������� �� ������.
        ///
        /// @param cfg_file - ���������� ��������� ���������� ����� � ���������
        ///   ����������.
        ///
        /// @return -  ���������� ��������� ����.
        virtual int load( file *cfg_file );

    protected:
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

        /// @brief ��������� ��������� ������ ����������� ������.
        ///
        /// @param index - ������ ������ � ������� ���������� �������� ������� 
        /// ����������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        float get_AO( u_int index, float min_value, float max_value );

        /// @brief ��������� ��������� ������ ����������� ������.
        ///
        /// @param index - ������ ������ � ������� ���������� �������� ������� 
        /// ����������.
        /// @param value - ����� ��������� ������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        int set_AO( u_int index, float value, float min_value = 0,
            float max_value = 0 );

        /// @brief ��������� ��������� ������ ����������� �����.
        ///
        /// @param index - ������ ������ � ������� ���������� ������� ������� 
        /// ����������.
        ///
        /// @return -  0 - ��.
        /// @return - >0 - ������.
        float get_AI( u_int index, float min_value = 0, float max_value = 0 );

        /// @brief ��������� �������� ���������.
        ///
        /// @param index - ������ ���������.
        ///
        /// @return - �������� ���������.
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
        /// ������ ��������.
        /// 
        /// @param str - ������ ��� ���������� ����������.
        /// @param [out] channels - ������, � ������� �����������.
        ///
        /// @return -  ���������� ��������� ����.
        int load_table_from_string( char *str, IO_channels &channels );
    };
//-----------------------------------------------------------------------------
/// @brief ������ � �������� �����/������ Wago.
///
/// ���������� ������ � ������ ��������� ������� �����/������ Wago.
class wago_manager
    {
    public:
        /// @brief ������ ������� �����.
        ///
        /// @return - 0 - ��.
        virtual int read_inputs() = 0;

        /// @brief ������ ������� ������.
        ///
        /// @return - 0 - ��.
        virtual int write_outputs() = 0;

        /// @brief �������� ������������ �� �����.
        ///
        /// ��������� � ����� ������ ��������� �� ������ ��������������� 
        /// ������.
        ///
        /// @param cfg_file - ���������������� ����.
        ///
        /// @return - 0 - ��.
        int load_from_cfg_file( file *cfg_file );

        /// @brief ��������� ������������� ���������� ������.
        static wago_manager* get_instance();

        /// @brief ��������� ������������� ���������� ������.
        static void set_instance( wago_manager* new_instance );

        /// @brief ��������� ������� ������ ��������� ������ ����������� �����.
        ///
        /// @param node_n - ����� ����.
        /// @param offset - �������� � �������� ����.
        ///
        /// @return - ��������� �� ������ ������.
        u_char* get_DI_read_data( u_int node_n, u_int offset );

        /// @brief ��������� ������� ������ ������ ��������� ������
        /// ����������� ������.
        ///
        /// @param node_n - ����� ����.
        /// @param offset - �������� � �������� ����.
        ///
        /// @return - ��������� �� ������ ������.
        u_char* get_DO_read_data( u_int node_n, u_int offset );

        /// @brief ��������� ������� ������ ������ ��������� ������
        /// ����������� ������.
        ///
        /// @param node_n - ����� ����.
        /// @param offset - �������� � �������� ����.
        ///
        /// @return - ��������� �� ������ ������.
        u_char* get_DO_write_data( u_int node_n, u_int offset );

        /// @brief ��������� ������� ������ ��������� ������ ����������� �����.
        ///
        /// @param node_n - ����� ����.
        /// @param offset - �������� � �������� ����.
        ///
        /// @return - ��������� �� ������ ������.
        u_int* get_AI_read_data( u_int node_n, u_int offset );

        /// @brief ��������� ������� ������ ������ ��������� ������
        /// ����������� ������.
        ///
        /// @param node_n - ����� ����.
        /// @param offset - �������� � �������� ����.
        ///
        /// @return - ��������� �� ������ ������.
        u_int* get_AO_read_data( u_int node_n, u_int offset );

        /// @brief ��������� ������� ������ ������ ��������� ������
        /// ����������� ������.
        ///
        /// @param node_n - ����� ����.
        /// @param offset - �������� � �������� ����.
        ///
        /// @return - ��������� �� ������ ������.
        u_int* get_AO_write_data( u_int node_n, u_int offset );

    protected:
        //---------------------------------------------------------------------
        /// @brief ���� ������� �����/������ Wago.
        //
        ///
        struct wago_node
            {
            wago_node();

            /// @brief �������� ������������ �� �����.
            ///
            /// ��������� � ����� ������ ��������� �� ������ ��������������� 
            /// ������.
            ///
            /// @param cfg_file - ���������������� ����.
            ///
            /// @return - 0 - ��.
            int load_from_cfg_file( file *cfg_file );

            int     state;          ///< C�������� ������ � �����.
            u_int   number;         ///< �����.
            u_int   type;           ///< ���.
            int     ip_addres[ 4 ]; ///< IP-�����.

            // Digital outputs ( DO ).
            u_int  DO_cnt;      ///< Amount of DO.
            u_char *DO;         ///< Current values.
            u_char *DO_;        ///< To write.

            // Analog outputs ( AO ).
            u_int AO_cnt;       ///< Amount of AO.
            u_int *AO;          ///< Current values.
            u_int *AO_;         ///< To write.
            u_int *AO_offsets;  ///< Offsets in common data.
            u_int *AO_types;    ///< Channels type.

            // Digital inputs ( DI ).
            u_int  DI_cnt;      ///< Amount of DI.
            u_char *DI;         ///< Current values.

            // Analog inputs ( AI ).
            u_int AI_cnt;       ///< Amount of AI.
            u_int *AI;          ///< Current values.
            u_int *AI_offsets;  ///< Offsets in common data.
            u_int *AI_types;    ///< Channels type.
            };
        //---------------------------------------------------------------------

        u_int       nodes_count;        ///< ���������� �����.
        wago_node **nodes;              ///< ����.

        static wago_manager* instance;  ///< ������������ ��������� ������.

public:
        wago_node * get_node( int node_n )
            {
            return nodes[ node_n ];
            }
    };
//-----------------------------------------------------------------------------
#define G_WAGO_MANAGER wago_manager::get_instance()
//-----------------------------------------------------------------------------
#endif // WAGO_H 

