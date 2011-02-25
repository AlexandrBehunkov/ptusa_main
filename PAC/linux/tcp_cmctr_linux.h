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

#include "tcp_cmctr.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
/// @brief C�������� ������.
struct socket_state
    {
    int active;      ///< ����� �������.
    int init;        ///< ����� ������ ��� ��� �����������.
    int is_listener; ///< ����� �������� ����������� ���������� ( = 0 )/����� �������� ���������� ( != 0 ).
    int evaluated;   ///< � ������ ����� ��� ��������� ����� ����������� �� ������� ������.
    int clID;        ///< ������������� ������� ��� ������������� ����, �� ����� �� ��� ����� ������ ��������.
    sockaddr_in sin; ///< ����� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������������ ��� Linux - ����� ������� PAC<->������.
class tcp_communicator_linux : public tcp_communicator 
    {
        public:
            tcp_communicator_linux( const char *name );
            virtual ~tcp_communicator_linux();

            /// @brief �������� ������ ������� � ��������.
            int evaluate();

	private:
            sockaddr_in ssin; 	        ///< ����� �������.
            u_int       sin_len;    	///< ����� ������.
            int         master_socket;	///< ������-����� ��� �������������.

#ifdef MODBUS
            int modbus_socket;          ///< ������ �����.
#endif
            int slave_socket; ///< �����-�����, ���������� ��� ����������� �������.
            int rc; ///< ��� �������� select.

            /// @brief ������� �������� ������ �� ������.
            ///
            /// @param skt - �����.
            int do_echo( int skt );
            
            u_long glob_last_transfer_time;  ///< ����� ��������� �������� �������� ������.

            timeval tv;                      ///< �������� �������� ������� ������ ��������� �������, 0 �� ���������.
            fd_set rfds;                     ///< ����� ������������ �������� ������ ��� ������.
            socket_state sst[ MAX_SOCKETS ]; ///< ������� ��������� �������.
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
//-----------------------------------------------------------------------------
#endif //TCP_CMCTR_LINUX
