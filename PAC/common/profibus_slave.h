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
    static profibus_slave* get_instance()
        {
#ifdef USE_PROFIBUS_SLAVE_PFC200
        return profibus_slave_PFC200::get_instance();
#endif // USE_PROFIBUS_SLAVE_PFC200

        return 0;
        }

    //��������� � Lua.
    virtual double get_double( int offset ) = 0;
    };
//-----------------------------------------------------------------------------
profibus_slave* G_PROFIBUS_SLAVE_LUA()
    {
    return profibus_slave::get_instance();
    }
#endif // PROFIBUS_SLAVE
