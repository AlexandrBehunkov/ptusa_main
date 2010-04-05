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

typedef struct socket_state
	{
	int active;    ///< ����� �������.
	int init;      ///< ����� ������ ��� ��� �����������.
	int islistener;///< ����� �������� ����������� ���������� ( = 0 )/����� �������� ���������� ( != 0 ).
	int evaluated; ///< � ������ ����� ��� ��������� ����� ����������� �� ������� ������.
	int clID;      ///< ������������� ������� ��� ������������� ����, �� ����� �� ��� ����� ������ ��������.
	struct sockaddr_in sin;       ///< ����� �������.
	} socket_state;

/// @brief ������������ ��� PAC WAGO 750 - ����� ������� PAC-������.
class tcp_communicator_w750 : public tcp_communicator 
	{
	public:
        tcp_communicator_w750();
		~tcp_communicator_w750();

	private:
		struct sockaddr_in 	ssin; 			///< ����� �������.
		unsigned int 		sin_len;    	///< ����� ������.
		int 				master_socket;	///< ������-����� ��� �������������.

#ifdef MODBUS
		int modbus_socket;				     ///< ������ �����.
#endif
		int ss;                  ///< �����-�����, ���������� ��� ����������� �������.
		int rc;                  ///< ��� �������� selectsocket.
		
        int do_echo( int skt );
		int netInit();

		int reboot;
		int glob_cmctr_ok;
		unsigned char* buf;
		int max_cycles;                     ///< ������������ ���������� ������ ��������� ��������� ������� �� 1 ������.
		timeval tv;                         ///< �������� �������� ������� ������ ��������� ������� 0 �� ���������.
		fd_set rfds;                        ///< ����� ������������ �������� ������ ��� ������.
		int netOK;
		int tcpipClientID;

		void killsockets ();
		int net_init();
		void net_terminate();
		
		int recvtimeout( uint s, uchar *buf, int len, int timeout, int usec );
	};
	
#endif //TCP_CMCTR_W750
