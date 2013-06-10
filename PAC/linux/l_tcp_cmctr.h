/// @file l_tcp_cmctr.h
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

        public:
            int sendall (int sockfd, unsigned char *buf, int len, long fsize)
                {
                int n = 1;
                unsigned char *p = buf;

                for ( int i = len; i > 0; )
                    {
                    if ( ( n = send( sockfd, p, i, 0 ) ) < 0 )
                        break;

                    i -= n;
                    p += n;
                    }

                return n;
                }

            /// @brief ��������� ������ � ���������.
            ///
            /// @param s        - �����.
            /// @param buf      - ����� ��� ������ ���������� ������.
            /// @param len      - ���������� ����������� ����.
            /// @param timeout  - ����� ��������, ���.
            /// @param usec     - ����� ��������, �� ���.
            /// @param usec     - ����� ��������, �� ���.
            /// @param IP       - IP-����� ���������.
            ///
            /// @return -1   - ������ ������ � �������.
            /// @return -2   - ������ ��������.
            /// @return >= 0 - ������ ������� ��������� ������.
            static int  recvtimeout( int s, u_char *buf, int len,
                int sec, int usec, char* IP );
	};
//-----------------------------------------------------------------------------
#endif //TCP_CMCTR_LINUX
