/// @file profibus_slave.h
/// @brief �������� �������� �������, ������� ��������� ���������� ����� PAC
/// �� ������ � Profibus Slave.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @par ������� ������:
/// @$Rev: 220 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-15 16:58:56#$.

#ifndef PROFIBUS_SLAVE
#define PROFIBUS_SLAVE

//-----------------------------------------------------------------------------
/// @brief ������ � Profibus Slave.
class profibus_slave
    {
    //���������������� �������.
    public:
        /// <summary>
        /// ��������� ������ ������.
        /// </summary>
        void activate()
            {
            active = true;
            }

        /// <summary>
        /// ��������� ������ �������.
        /// </summary>
        void set_station_address( int address )
            {
            station_address = address;
            }

        /// <summary>
        /// ��������� ������� ������� ������� ������.
        /// </summary>
        void set_output_byte_size( int size )
            {
            output_byte_size = size;
            }

        /// <summary>
        /// ��������� ������� ������� ������� ������.
        /// </summary>
        void set_input_byte_size( int size )
            {
            input_byte_size = size;
            }

    public:
        profibus_slave()
            {
            active = false;
            station_address = 2;
            output_byte_size = 1;
            input_byte_size = 1;
            }

        inline bool is_active() const
            {
            return active;
            };

        virtual int init() = 0;
        virtual int eval() = 0;

    public:
        /// @brief ��������� ������������� ���������� ������ ��� ������.
        ///
        /// @return - ��������� �� ������������ ������ ������.
        static profibus_slave* get_instance();

        /// <summary>
        /// ��������� �������� ���� double.
        /// </summary>
        /// <param name="offset">��������, �������� 0..239.</param>
        virtual double get_double( int offset ) = 0;

        /// <summary>
        /// ��������� �������� ���� bool.
        /// </summary>
        /// <param name="byte_offset">��������, �������� 0..243.</param>
        /// <param name="bit_offset">��������, �������� 0..7.</param>
        virtual bool get_bool( int byte_offset, int bit_offset ) = 0;

        /// <summary>
        /// ��������� �������� ���� bool.
        /// </summary>
        /// <param name="byte_offset">��������, �������� 0..243.</param>
        /// <param name="bit_offset">��������, �������� 0..7.</param>
        /// <param name="val">��������.</param>
        virtual void set_bool( int byte_offset, int bit_offset, bool val ) = 0;

        /// <summary>
        /// ��������� �������� ���� int.
        /// </summary>
        /// <param name="byte_offset">��������, �������� 0..242.</param>
        virtual int get_int( int byte_offset ) = 0;

        /// <summary>
        /// ��������� �������� ���� int.
        /// </summary>
        /// <param name="byte_offset">��������, �������� 0..242.</param>
        /// <param name="val">��������.</param>
        virtual void set_int( int byte_offset, int val ) = 0;

    private:
        bool active;
        int station_address;
        int output_byte_size;
        int input_byte_size;
    };
//-----------------------------------------------------------------------------
profibus_slave* G_PROFIBUS_SLAVE_LUA();
profibus_slave* G_PROFIBUS_SLAVE();

#endif // PROFIBUS_SLAVE
