/// @file mos7_tcp_cmctr.h
/// @brief �������� �������� �������, ������� ������������� ��� ������ �������
/// PAC-������ �� ��������� TCP ��� OC Windows.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev: 225 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-24 17:29:00#$.

#ifndef TCP_CMCTR_LINUX
#define TCP_CMCTR_LINUX

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
//    sockaddr_in sin; ///< ����� �������.
    };
//-----------------------------------------------------------------------------
/// @brief ������������ ��� Windows - ����� ������� PAC<->������.
class tcp_communicator_mos7 : public tcp_communicator 
    {
        public:
            tcp_communicator_mos7( const char *name )
                {
                }

            virtual ~tcp_communicator_mos7()
                {

                }

            /// @brief �������� ������ ������� � ��������.
            int evaluate()
                {

                }

    private:
            //sockaddr_in ssin; 	        ///< ����� �������.
            int         sin_len;    	///< ����� ������.
            int         master_socket;  ///< ������-����� ��� �������������.

#ifdef MODBUS
            int modbus_socket;  ///< ������ �����.
#endif
            int rc;             ///< ��� �������� select.

            /// @brief ������� �������� ������ �� ������.
            ///
            /// @param skt - �����.
            int do_echo( int skt )
                {

                }
            
            u_long glob_last_transfer_time;  ///< ����� ��������� �������� �������� ������.

            //timeval tv;                      ///< �������� �������� ������� ������ ��������� �������, 0 �� ���������.
            //fd_set rfds;                     ///< ����� ������������ �������� ������ ��� ������.
            std::vector< socket_state > sst; ///< ������� ��������� �������.
            int netOK;                       ///< ������� �������� ������������� ����.

            /// @brief ����������� �������.
            void killsockets ()
                {

                }

            /// @brief ������������� ����.
            int  net_init()
                {

                }

            /// @brief �������� ����.
            void net_terminate()
                {

                }

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
                int timeout, int usec )
                {

                }
    };
    
#endif //TCP_CMCTR_LINUX
