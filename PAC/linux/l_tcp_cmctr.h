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
#include <vector>
//-----------------------------------------------------------------------------
/// @brief C�������� ������.
struct socket_state
    {
    int socket;
    int active;      ///< ����� �������.
    int init;        ///< ����� ������ ��� ��� �����������.
    int is_listener; ///< ����� �������� ����������� ���������� ( = 0 )/����� �������� ���������� ( != 0 ).
    int evaluated;   ///< � ������ ����� ��� ��������� ����� ����������� �� ������� ������.
    int ismodbus;
    sockaddr_in sin; ///< ����� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������������ ��� Linux - ����� ������� PAC<->������.
class tcp_communicator_linux : public tcp_communicator
    {
        public:
            tcp_communicator_linux( const char *name_rus,
                const char *name_eng );
            virtual ~tcp_communicator_linux();

            /// @brief �������� ������ ������� � ��������.
            int evaluate();

    private:
            sockaddr_in ssin; 	        ///< ����� �������.
            u_int       sin_len;    	///< ����� ������.
            int         master_socket;	///< ������-����� ��� �������������.

            int modbus_socket;          ///< ������ �����.
            int slave_socket; ///< �����-�����, ���������� ��� ����������� �������.
            int rc; ///< ��� �������� select.

            /// @brief ������� �������� ������ �� ������.
            ///
            /// @param skt - �����.
            int do_echo( int idx );

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

        public:
            int sendall (int sockfd, unsigned char *buf, int len, long fsize)
                {
                int total_size = 0;
                int n = 1;
                unsigned char *p = buf;

                for ( int i = len; i > 0; )
                    {
                    static int err_cnt = 0;
                    if ( ( n = send( sockfd, p, i, 0 ) ) < 0 )
                        {
                        err_cnt++;
                        usleep( 100000 );
                        if ( err_cnt > 3 )
                            {
#ifdef DEBUG
//                            Print( "tcp_communicator_linux::send_all(): "
//                                "send %i byte of %d.\n", total_size, len );
#endif
                            break;
                            }

                        continue;
                        }

                    err_cnt = 0;
                    i -= n;
                    p += n;

                    total_size += n;
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
