/// @file tcp_cmctr_w750.h
/// @brief �������� �������� �������, ������� ������������� ��� ������ �������
/// PAC-������ �� ��������� TCP ��� PAC WAGO 750.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev: 39 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-04-02 17:22:01#$.

#ifndef TCP_CMCTR_W750
#define TCP_CMCTR_W750

#include "tcp_cmctr.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
/// @brief ��������� ������.
struct socket_state
    {
    int active;      ///< ����� �������.
    int init;        ///< ����� ������ ��� ��� �����������.
    int islistener;  ///< ����� �������� ����������� ���������� ( = 0 )/����� �������� ���������� ( != 0 ).
    int evaluated;   ///< � ������ ����� ��� ��������� ����� ����������� �� ������� ������.
    int clID;        ///< ������������� ������� ��� ������������� ����, �� ����� �� ��� ����� ������ ��������.
    sockaddr_in sin; ///< ����� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������������ ��� PAC WAGO 750 - ����� ������� PAC-������.
class tcp_communicator_w750 : public tcp_communicator 
    {
        public:
            tcp_communicator_w750();
            virtual ~tcp_communicator_w750();

            /// @brief �������� ������ ������� � ��������.
            int evaluate();

	private:
            sockaddr_in ssin; 	        ///< ����� �������.
            u_int       sin_len;    	///< ����� ������.
            int         master_socket;	///< ������-����� ��� �������������.

#ifdef MODBUS
            int modbus_socket;          ///< ������ �����.
#endif
            int ss; ///< �����-�����, ���������� ��� ����������� �������.
            int rc; ///< ��� �������� selectsocket.

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
            int  recvtimeout( uint s, u_char *buf, int len,
                int timeout, int usec );
	};
	
#endif //TCP_CMCTR_W750
