/// @file tcp_cmctr_linux.h
/// @brief �������� �������� �������, ������� ������������� ��� ������ �������
/// PAC-������ �� ��������� TCP ��� OC Linux.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef TCP_CMCTR_LINUX
#define TCP_CMCTR_LINUX

#include <winsock2.h>
#include <vector>

#include "tcp_cmctr.h"
//-----------------------------------------------------------------------------
/// @brief C�������� ������.
struct socket_state
    {
    int         socket;
    int         active;      ///< ����� �������.
    int         init;        ///< ����� ������ ��� ��� �����������.
    int         is_listener; ///< ����� �������� ����������� ���������� ( = 0 )/����� �������� ���������� ( != 0 ).
    int         evaluated;   ///< � ������ ����� ��� ��������� ����� ����������� �� ������� ������.
    int         clID;        ///< ������������� ������� ��� ������������� ����, �� ����� �� ��� ����� ������ ��������.
    sockaddr_in sin; ///< ����� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������������ ��� Linux - ����� ������� PAC<->������.
class tcp_communicator_win : public tcp_communicator 
    {
        public:
            tcp_communicator_win( const char *name );
            virtual ~tcp_communicator_win();

            /// @brief �������� ������ ������� � ��������.
            int evaluate();

    private:
            sockaddr_in ssin; 	        ///< ����� �������.
            int         sin_len;    	///< ����� ������.
            int         master_socket;  ///< ������-����� ��� �������������.

#ifdef MODBUS
            int modbus_socket;  ///< ������ �����.
#endif
            int rc;             ///< ��� �������� select.

            /// @brief ������� �������� ������ �� ������.
            ///
            /// @param skt - �����.
            int do_echo( int skt );
            
            u_long glob_last_transfer_time;  ///< ����� ��������� �������� �������� ������.

            timeval tv;                      ///< �������� �������� ������� ������ ��������� �������, 0 �� ���������.
            fd_set rfds;                     ///< ����� ������������ �������� ������ ��� ������.
            std::vector< socket_state > sst; ///< ������� ��������� �������.
            int netOK;                       ///< ������� �������� ������������� ����.

            /// @brief ����������� �������.
            void killsockets ();

            /// @brief ������������� ����.
            int  net_init();

            /// @brief �������� ����.
            void net_terminate();

            /// @brief ��������� ������ � ���������.
            ///
            /// @param s        - �����.
            /// @param buf      - ����� ��� ������ ���������� ������.
            /// @param len      - ���������� ����������� ����.
            /// @param timeout  - ����� ��������, ���.
            /// @param usec     - ����� ��������, �� ���.
            ///
            /// @return -1   - ������ ������ � �������.
            /// @return -2   - ������ ��������.
            /// @return >= 0 - ������ ������� ��������� ������.
            int  recvtimeout( u_int s, u_char *buf, int len,
                int timeout, int usec );
    };
    
#endif //TCP_CMCTR_LINUX
