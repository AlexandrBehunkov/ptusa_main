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
public:
    /// @brief ��������� ������������� ���������� ������ ��� ������.
    ///
    /// @return - ��������� �� ������������ ������ ������ @ref
    /// profibus_slave_PFC200.
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
    };
//-----------------------------------------------------------------------------
profibus_slave* G_PROFIBUS_SLAVE_LUA();

#endif // PROFIBUS_SLAVE
