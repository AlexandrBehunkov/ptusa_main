#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <tchar.h>

#include "tcp_cmctr_win.h"
#include "PAC_err.h"
//------------------------------------------------------------------------------
#ifdef DEBUG
unsigned int max_buffer_use = 0;
#endif
//------------------------------------------------------------------------------
char* WSA_Err_Decode( int ErrCode );
//------------------------------------------------------------------------------
tcp_communicator_win::tcp_communicator_win( const char *name ):
tcp_communicator(),
    netOK( 0 )
    {
    // ������ �������.
    tv.tv_sec  = 0;
    tv.tv_usec = 10000; // 0.01 ���.

    sin_len = sizeof( ssin );
    strncpy( host_name, name, TC_MAX_HOST_NAME );

    net_init();

#ifdef DEBUG
    char tmp_host_name[ TC_MAX_HOST_NAME ] = { 0 };
    gethostname( tmp_host_name, TC_MAX_HOST_NAME );
    printf ( "tcp_communicator_linux() - host name is \"%s\".\n",
        tmp_host_name );
    printf ( "PAC name \"%s\".\n", host_name );
#endif // DEBUG


    glob_last_transfer_time = get_sec();
    }
//------------------------------------------------------------------------------
void tcp_communicator_win::killsockets()
    {
    FD_ZERO( &rfds ); /* clear FD set */

    for ( u_int i = 0; i < sst.size(); i++ )
        {
        if ( sst[ i ].active )
            {
            sst[ i ].clID = -1;
            shutdown( sst[ i ].socket, SD_BOTH );
            closesocket( sst[ i ].socket );
            }
        }

    sst.clear();
    }
//------------------------------------------------------------------------------
int tcp_communicator_win::net_init()
    {
    WSAData tmp_WSA_data;

    if ( WSAStartup( 0x202, &tmp_WSA_data ) )
        {
        Print( "������ ������������� ������� ����������: %s\n",
            WSA_Err_Decode( WSAGetLastError() ) );
        return -1;
        }

    int type     = SOCK_STREAM;
    int protocol = 0;        /* ������ 0 */
    //-C������� ������-������.
    master_socket = socket( PF_INET, type, protocol ); 

    if ( master_socket < 0 )
        {
#ifdef DEBUG
        Print( "tcp_communicator_linux:net_init() - can't create master socket: %s\n",
            WSA_Err_Decode( WSAGetLastError() ) );
#endif // DEBUG
        return -4;
        }

#ifdef DEBUG
    printf( "tcp_communicator_linux:net_init() - master socket created. Has number %d\n\r",
        master_socket );
#endif // DEBUG

    //-��������� � ������������� �����.
    u_long mode = 0;
    int res = ioctlsocket( master_socket, FIONBIO, &mode );
    if ( res == SOCKET_ERROR )
        { 
        Print( "tcp_communicator_linux:net_init() - ������  ������  ioctlsocket: %s\n",
            WSA_Err_Decode( WSAGetLastError() ) );
        closesocket( master_socket );
        return -1;
        }

    //-��������� ������-������.
    socket_state master_socket_state;
    memset( &master_socket_state.sin, 0, sizeof( master_socket_state.sin ) );
    master_socket_state.sin.sin_family 	    = AF_INET;
    master_socket_state.sin.sin_addr.s_addr = INADDR_ANY;
    master_socket_state.sin.sin_port 		= htons( PORT );
    master_socket_state.socket              = master_socket;

    master_socket_state.active      = 1; // ������-����� ������ ��������.
    master_socket_state.is_listener = 1; // ����� �������� ����������.
    master_socket_state.evaluated   = 0;

    sst.push_back( master_socket_state );

    const int on = 1;
    if ( setsockopt( master_socket, SOL_SOCKET, SO_REUSEADDR, 
        ( char* ) &on, sizeof( on ) ) )
        {
        Print( "tcp_communicator_linux:net_init() - ������  ������  setsockopt: %s\n",
            WSA_Err_Decode( WSAGetLastError() ) );        
        closesocket( master_socket );
        return -5;
        }

    //-�������� ������.
    int err = bind( master_socket, ( struct sockaddr * ) 
        & master_socket_state.sin, sizeof( master_socket_state.sin ) );
    if ( err < 0 )
        {
#ifdef DEBUG
        Print( "tcp_communicator_linux:net_init() - can't bind master socket to port %d : %s\n",
            PORT, WSA_Err_Decode( WSAGetLastError() ) );
#endif // DEBUG
        closesocket( master_socket );
        return -5;
        }

    err = listen( master_socket, QLEN ); // ������ ������-����� ����������.
    if ( type == SOCK_STREAM && err < 0 )
        {
        closesocket( master_socket );
#ifdef DEBUG
        Print( "tcp_communicator_linux:net_init() - listen: %s\n",
            WSA_Err_Decode( WSAGetLastError() ) );        
#endif // DEBUG
        return -6;
        }

    int val = 1;
    setsockopt( master_socket, SOL_SOCKET, SO_REUSEADDR, 
        ( char* ) &val, sizeof( val ) );
#ifdef MODBUS
    // �������� ���������� ������ modbus_socket.
    err = modbus_socket = socket ( PF_INET, type, protocol );

#ifdef DEBUG
    printf( "tcp_communicator_linux:net_init() - modbus socket created. Has number %d\n\r",
        modbus_socket );
#endif // DEBUG

    if ( modbus_socket < 0 )
        {
#ifdef DEBUG
        perror( "tcp_communicator_linux:net_init() - can't create modbus socket" );
#endif //DEBUG

        return -4;
        }
    // ��������� modbus_socket ������.
    socket_state modbus_socket_state;
    memset( &modbus_socket_state.sin, 0, sizeof ( modbus_socket_state.sin ) );
    modbus_socket_state.sin.sin_family 	    = AF_INET;
    modbus_socket_state.sin.sin_addr.s_addr = 0;
    modbus_socket_state.sin.sin_port 		= htons ( 502 ); // ����.

    modbus_socket_state.active      = 1;
    modbus_socket_state.is_listener = 1;
    modbus_socket_state.evaluated   = 0;

    sst.push_back( modbus_socket_state );

    err = bind( modbus_socket, ( struct sockaddr * ) & modbus_socket_state.sin,
        sizeof ( modbus_socket_state.sin ) );	   // �������� ������.
    if ( err < 0 )
        {
#ifdef DEBUG
        printf( "tcp_communicator_linux:net_init() - can't bind modbus socket to port %d : %s\n",
            502, strerror( errno ) );
#endif // DEBUG

        closesocket( modbus_socket );
        return -5;
        }
    err = listen( modbus_socket, QLEN ); // ������ ����������.
    if ( type == SOCK_STREAM && err < 0 )
        {
        closesocket( modbus_socket );
#ifdef DEBUG
        perror( "tcp_communicator_linux:net_init() - listen" );
#endif // DEBUG
        return -6;
        }
#endif //MODBUS

    netOK = 1;
    return 0;
    }
//------------------------------------------------------------------------------
void tcp_communicator_win::net_terminate()
    {
    killsockets();
    netOK = 0;
    }
//------------------------------------------------------------------------------
tcp_communicator_win::~tcp_communicator_win()
    {
    net_terminate();
    WSACleanup();
    }
//------------------------------------------------------------------------------
int tcp_communicator_win::evaluate()
    {
    // �������� ����� � ��������.
    if ( get_sec() - glob_last_transfer_time > 5 )
        {
        if ( glob_cmctr_ok )
            {
            glob_cmctr_ok = 0;
            PAC_critical_errors_manager::get_instance()->set_global_error(
                EC_NO_CONNECTION, ES_EASYSERVER, ES_EASYSERVER );
            }
        }
    else
        {
        if ( glob_cmctr_ok == 0 )
            {
            glob_cmctr_ok = 1;
            PAC_critical_errors_manager::get_instance()->reset_global_error(
                EC_NO_CONNECTION, ES_EASYSERVER, ES_EASYSERVER );
            }
        }
    // �������� ����� � ��������.-!>

    // ������������� ����, ��� �������������.
    if ( !netOK )
        {
        net_init();
        if ( !netOK ) return -100;
        }
    // ������������� ����, ��� �������������.-!>

    int count_cycles = 0;
    while ( count_cycles < max_cycles )
        {
        /* service loop */
        count_cycles++;

        FD_ZERO( &rfds );
        for ( u_int i = 0; i < sst.size(); i++ )
            {
            if ( sst[ i ].active && 
                sst[ i ].is_listener &&
                !sst[ i ].evaluated )
                {
                //- Re-join active sockets.
                FD_SET( sst[ i ].socket, &rfds );
                }
            }

        //-��� ������� � ����� �� �������.
        rc = select( 0/*�� �����������*/, &rfds, NULL, NULL, &tv );

        if ( 0 == rc ) continue; // ������ �� ���������.

        if ( rc < 0 )
            {
#ifdef DEBUG     
            Print( "������ select: %s\n", WSA_Err_Decode( WSAGetLastError() ) );
#endif
            continue;
            }

        for ( u_int i = 0; i < sst.size(); i++ )  /* scan all possible sockets */
            {
            // �������� ����� ������ �� ����������.
            if ( FD_ISSET ( sst[ i ].socket, &rfds ) )
                {
#ifndef MODBUS
                if ( sst[ i ].socket == master_socket )
#else
                if ( sst[ i ].socket == master_socket || 
                    sst[ i ].socket == modbus_socket )
#endif
                    {
                    /* master socket */
                    memset( &ssin, 0, sizeof ( ssin ) );
                    int slave_socket = accept ( sst[ i ].socket, 
                        ( struct sockaddr * ) &ssin, &sin_len );

                    if ( slave_socket <= 0 )    // ������.
                        {
#ifdef DEBUG
                        Print( "������ accept(): %s\n",
                            WSA_Err_Decode( WSAGetLastError() ) );                        
#endif                        
                        continue;   
                        }
                    // ��������� ������ � ������������� �����.
                    u_long mode = 0;
                    if ( ioctlsocket( slave_socket, FIONBIO, &mode ) == SOCKET_ERROR ) 
                        {
                        Print( "������ �������� ����������� ������ � ������������� �����: %s\n",
                            WSA_Err_Decode( WSAGetLastError() ) );
                        // ������, ��������� �����.
                        shutdown( slave_socket, SD_BOTH );
                        closesocket( slave_socket );
                        continue;
                        }
#ifdef DEBUG
                    // ����������� ����� �������.
                    hostent *client = gethostbyaddr( ( char* ) &ssin.sin_addr, 4, AF_INET );

                    if ( client )
                        {
                        printf( "Accepted connection on %d socket from %s [ %s ].\n",
                            slave_socket, inet_ntoa( ssin.sin_addr ), client->h_name  );
                        }
                    else
                        {
                        printf( "Error getting client name. " );
                        printf( "Accepted connection on %d socket from %s.\n",
                            slave_socket, inet_ntoa( ssin.sin_addr ) );
                        }
#endif // DEBUG

#ifdef MODBUS
                    if ( i != modbus_socket )
                        {
#endif
                        char Message1[] = "PAC accept";
                        send( slave_socket, Message1, strlen ( Message1 ), 0 );
#ifdef MODBUS
                        }
#endif
                    FD_SET( slave_socket, &rfds );
                    socket_state slave_socket_state;
                    slave_socket_state.active = 1;
                    slave_socket_state.init   = 1;
                    slave_socket_state.is_listener = 1;
                    slave_socket_state.evaluated = 0;
                    slave_socket_state.socket = slave_socket;

                    memcpy( &slave_socket_state.sin, &ssin, sin_len );

                    sst.push_back( slave_socket_state );
                    }
                else         /* slave socket */
                    {
                    do_echo ( i );
                    glob_last_transfer_time = get_sec();
                    }
                }
            }
        }  /* service loop */

    for ( u_int i = 0; i < sst.size(); i++ )
        {
        sst[ i ].evaluated = 0;
        }

    return 0;
    }
//------------------------------------------------------------------------------
int tcp_communicator_win::recvtimeout( u_int s, u_char *buf,
    int len, int timeout, int usec )
    {
    // �����������  file descriptor set.
    fd_set fds;
    FD_ZERO( &fds );
    FD_SET( s, &fds );

    // ����������� ����� �� �������.
    timeval rec_tv;
    rec_tv.tv_sec = timeout;
    rec_tv.tv_usec = usec;

    // ���� �������� ��� ���������� ������.
    int n = select( s + 1, &fds, NULL, NULL, &rec_tv );
    if ( 0 == n ) return -2;  // timeout!
    if ( -1 == n ) return -1; // error

    // ������ ������ ���� �����, ������� ������ ������� recv().    
    return recv( s, ( char* ) buf, len, 0 );
    }
//------------------------------------------------------------------------------
int tcp_communicator_win::do_echo( int idx )
    {
    socket_state &sock_state = sst[ idx ];

    FD_CLR( sock_state.socket, &rfds );

    int err = 0, res;

    if ( sock_state.init )         /* socket is just initiated */
        {
        sock_state.init = 0;

#ifdef  MODBUS
        return err;
#endif // MODBUS
        }

    sock_state.evaluated = 1;
    memset( buf, 0, BUFSIZE );

    // ������� ������ � ��������� 1 ���.
    err = in_buffer_count = recvtimeout( sock_state.socket, buf, BUFSIZE, 1, 0 );

    if ( err <= 0 )               /* read error */
        {
#ifdef DEBUG
        switch ( err )
            {
        case 0:
            printf( "Socket %d->\"%s\" was closed.\n",
                sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ) );
            break;

        case -1:
            printf( "Socket %d->\"%s\" disconnected on read try : %s\n",
                sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ),
                WSA_Err_Decode( WSAGetLastError() ) );
            break;

        case -2:
            printf( "Socket %d->\"%s\" disconnected on read try - timeout.\n",
                sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ) );
            break;

        default:
            printf( "Socket %d->\"%s\" disconnected on read try : %s\n",
                sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ),
                WSA_Err_Decode( WSAGetLastError() ) );
            break;
            }
#endif // DEBUG

        shutdown( sock_state.socket, 0 );
        closesocket( sock_state.socket );
        sst.erase( sst.begin() + idx, sst.begin() + idx + 1 );
        return err;
        }

#ifdef DEBUG
    if ( in_buffer_count > max_buffer_use )
        {
        max_buffer_use = in_buffer_count;
        printf( "Max buffer use %u\n", max_buffer_use );
        }
#endif // DEBUG

    net_id = buf[ 0 ];
    pidx = buf[ 3 ];
    if ( net_id != 's' )
        {
#ifdef DEBUG
        printf( "Incorrect input data at socket %d->\"%s\"\n",
            sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ) );
#endif // DEBUG
        return ERR_RETRIVE;
        }

    if ( buf[ 1 ] < TC_MAX_SERVICE_NUMBER && services[ buf[ 1 ] ] != NULL &&
        ( buf[ 2 ] + buf[ 3 ] != 0 ) )
        {
        switch ( buf[ 2 ] )
            {
        case FRAME_SINGLE:
            res = services[ buf[ 1 ] ] ( 
                ( u_int ) ( buf[ 4 ] * 256 + buf[ 5 ] ), buf + 6, buf + 5 );

            if ( res == 0 )
                {
                _AknOK();
                }
            else
                {
                _AknData( res );
#ifdef DEBUG
                if ( ( unsigned int ) res > max_buffer_use )
                    {
                    max_buffer_use = res;
                    printf( "Max buffer use %u\n", res );
                    }
#endif
                }
            break;

        default:
            _ErrorAkn( ERR_WRONG_CMD );
#ifdef DEBUG
            printf( "Wrong command received on socket %d->\"%s\"\n",
                sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ) );
#endif // DEBUG
            break;
            }
        }
    else
        {
        if ( ( services[ 15 ] != NULL ) && ( 0 == buf[ 2 ] + buf[ 3 ] ) ) //MODBUS
            {
            res = services[ 15 ] ( ( u_int ) ( buf[ 4 ] * 256 + buf[ 5 ] ),
                buf + 6,
                buf + 6 );
            if ( res > 0 )
                {
                buf[ 4 ] = ( res >> 8 ) & 0xFF;
                buf[ 5 ] = res & 0xFF;
                in_buffer_count = res + 6;
                }
            sock_state.evaluated = 0;
            }
        else
            {
            _ErrorAkn( ERR_WRONG_SERVICE );
#ifdef DEBUG
            printf( "No such service %d at socket %d->\"%s\"\n",
                buf[ 1 ], sock_state.socket, 
                inet_ntoa( sock_state.sin.sin_addr ) );
#endif // DEBUG
            }
        }

    err = send( sock_state.socket, ( char* ) buf, in_buffer_count, 0 );
    if ( is_going_to_reboot )
        {
        killsockets();
        Sleep( 800000 );
        }

    if ( err <= 0 )               /* write error */
        {
#ifdef DEBUG
        printf( "Socket %d->\"%s\" disconnected on write try : %s\n",
            sock_state.socket, inet_ntoa( sock_state.sin.sin_addr ),
            WSA_Err_Decode( WSAGetLastError() ) );
#endif // DEBUG

        shutdown( sock_state.socket, 0 );
        closesocket( sock_state.socket );
        sst.erase( sst.begin() + idx, sst.begin() + idx + 1 );
        return err;
        }

    return err;
    }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
char* WSA_Err_Decode (int ErrCode)
    {
    switch (ErrCode)
        {
    case WSANOTINITIALISED:
        return "A successful WSAStartup must occur before using this function.";
    case WSAENETDOWN:
        return "The network subsystem has failed.";
    case WSAHOST_NOT_FOUND:
        return "Authoritative answer host not found.";
    case WSATRY_AGAIN:
        return "Non authoritative host not found, or server failure.";
    case WSANO_RECOVERY:
        return "A nonrecoverable error occurred.";
    case WSANO_DATA:
        return "Valid name, no data record of requested type.";
    case WSAEADDRINUSE:
        return "The specified address is already in use.";
    case WSAEINTR:
        return "The (blocking) call was canceled through WSACancelBlockingCall.";
    case WSAEAFNOSUPPORT:
        return "The specified address family is not supported.";
    case WSAECONNREFUSED:
        return "The attempt to connect was forcefully rejected.";
    case WSAEFAULT:
        return "The name or the namelen argument is not a valid part of the user \
address space, the namelen argument is too small, or the name \
argument contains incorrect address format for the associated \
address family. In case WSAStartup - lpWSAData is not a valid pointer";
    case WSAENOTCONN:
        return "The socket is not connected.";
    case WSAEINPROGRESS:
        return "A blocking Windows Sockets 1.1 call is in progress, or the service \
provider is still processing a callback function.";
    case WSAENETRESET:
        return "The connection has been broken due to the remote host resetting.";
    case WSAEALREADY:
        return "A nonblocking connect call is in progress on the specified socket. \
Note  In order to preserve backward compatibility, this error is \
reported as WSAEINVAL to Windows Sockets 1.1 applications that link \
to either WINSOCK.DLL or WSOCK32.DLL.";
    case WSAEADDRNOTAVAIL:
        return "The specified address is not available from the local machine.";
    case WSAEMFILE:
        return "No more socket descriptors are available.";
    case WSAENOBUFS:
        return "No buffer space is available. The socket cannot be created.";
    case WSAENOTSOCK:
        return "The descriptor is not a socket.";
    case WSAEOPNOTSUPP:
        return "MSG_OOB was specified, but the socket is not stream style such \
as type SOCK_STREAM, out-of-band data is not supported in the \
communication domain associated with this socket, or the socket \
is unidirectional and supports only send operations.";
    case WSAESHUTDOWN:
        return "The socket has been shut down; it is not possible to recv on a \
socket after shutdown has been invoked with how set to SD_RECEIVE \
or SD_BOTH.";
    case WSAETIMEDOUT:
        return "Attempt to connect timed out without establishing a connection.";
    case WSAECONNRESET:
        return "The virtual circuit was reset by the remote side \
executing a ""hard"" or ""abortive"" close. The \
application should close the socket as it is no \
longer usable. On a UDP datagram socket this error \
would indicate that a previous send operation resulted \
in an ICMP ""Port Unreachable"" message.";
    case WSAEWOULDBLOCK:
        return "The socket is marked as nonblocking and the connection cannot \
be completed immediately. It is possible to select the socket \
while it is connecting by selecting it for writing.";
    case WSAEMSGSIZE:
        return "The message was too large to fit into the specified buffer and \
was truncated.";
    case WSAEACCES:
        return "Attempt to connect datagram socket to broadcast address failed \
because setsockopt SO_BROADCAST is not enabled.";
    case WSAEPROTONOSUPPORT:
        return "The specified protocol is not supported.";
    case WSAEPROTOTYPE:
        return "The specified protocol is the wrong type for this socket.";
    case WSAESOCKTNOSUPPORT:
        return "The specified socket type is not supported in this address family.";
    case WSAEINVAL:
        return "The parameter g specified is not valid.";
    case WSAECONNABORTED:
        return "The virtual circuit was terminated due to a time-out or other \
failure. The application should close the socket as it is no \
longer usable.";
    case WSAEISCONN:
        return "The socket is already connected (connection-oriented sockets only).";
    case WSAENETUNREACH:
        return "The network cannot be reached from this host at this time.";
        //Errors, returned WSAStartup
    case WSASYSNOTREADY:
        return "Underlying network subsystem is not ready for network communication.";
    case WSAVERNOTSUPPORTED:
        return "The version of Windows Sockets support requested is not \
provided by this particular Windows Sockets implementation.";
    case WSAEPROCLIM:
        return "Limit on the number of tasks supported by the Windows Sockets \
implementation has been reached.";
        }
    return "Unknown error";
    }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
