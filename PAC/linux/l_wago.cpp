#include <bits/errno.h>

#include "l_wago.h"

extern int errno;
//-----------------------------------------------------------------------------
int wago_manager_linux::net_init( wago_node *node )
    {
    int type = SOCK_STREAM;
    int protocol = 0; /* ������ 0 */
    int err;
    int sock = err = socket( AF_INET, type, protocol ); // C������� ������.

    if( sock < 0 )
        {
#ifdef DEBUG
        perror( "wago_manager_linux:net_init(...) - can't create socket" );
#endif // DEBUG
        return -4;
        }

    // ��������� ������-������.
    struct sockaddr_in socket_remote_server;
    const int PORT = 502;
    memset( &socket_remote_server, 0, sizeof( socket_remote_server ) );
    socket_remote_server.sin_family 	 = AF_INET;
    socket_remote_server.sin_addr.s_addr = inet_addr( node->ip_address );
    socket_remote_server.sin_port        = htons ( PORT );

    const int C_ON = 1;

    if ( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &C_ON, sizeof( C_ON ) ) )
        {
        perror( "wago_manager_linux:net_init(...) - ������  ������  setsockopt" );
        close( sock );
        return -5;
        }

    // ��������� � ������������� �����.
    err = fcntl( sock, F_SETFL, O_NONBLOCK );
    if ( err != 0 )
        {
#ifdef DEBUG
        perror( "wago_manager_linux:net_init(...) - ������  ������  fcntl" );
#endif
        close( sock );
        return -5;
        }

    // �������� ������. ����� ���������� ���������� � ������������� ������.
    err = connect( sock, ( struct sockaddr * ) &socket_remote_server,
        sizeof( socket_remote_server ) );

    fd_set rdevents;
    struct timeval tv;
    FD_ZERO( &rdevents );
    FD_SET( sock, &rdevents );

    tv.tv_sec  = 0;
    tv.tv_usec = wago_node::C_CNT_TIMEOUT_US;

    err = select( sock + 1, 0, &rdevents, 0, &tv );

    if ( err <= 0 )
        {
#ifdef DEBUG
        if ( node->is_set_err == false )
            {
            if ( err < 0 )
                {
                printf( "wago_manager_linux:net_init(...) - can't connect to \"%s\":%d : %s\n",
                    node->ip_address, PORT, strerror( err ) );
                }
            else // = 0
                {
                printf( "wago_manager_linux:net_init(...) - can't connect to \"%s\":%d : timeout\n",
                    node->ip_address, PORT );
                }
            }
#endif // DEBUG

        close( sock );
        return -5;
        }

    if( FD_ISSET( sock, &rdevents ) )
        {
        socklen_t err_len;
        int error;

        err_len = sizeof( error );
        if( getsockopt( sock, SOL_SOCKET, SO_ERROR, &error, &err_len ) < 0 || error != 0 )
            {
#ifdef DEBUG
            if ( node->is_set_err == false )
                {
                perror( "wago_manager::net_init(...) select error" );
                }
#endif // DEBUG

            close( sock );
            return -6;
            }
        }

#ifdef DEBUG
    printf( "Socket %d is successfully connected to \"%s\":%d\n",
        sock, node->ip_address, PORT );
#endif // DEBUG

    node->sock   = sock;
    node->state  = wago_node::ST_OK;

    return 0;
    }
//-----------------------------------------------------------------------------
int wago_manager_linux::write_outputs()
    {
    if ( 0 == nodes_count ) return 0;

    for ( u_int i = 0; i < nodes_count; i++ )
        {
        wago_node *nd = nodes[ i ];
        if ( nd->type == wago_node::T_750_XXX_ETHERNET )
            {
            if ( !nd->is_active )
                {
                continue;
                }

            if ( nd->DO_cnt > 0 )
                {
                u_int bytes_cnt = nd->DO_cnt / 8 + ( nd->DO_cnt % 8 > 0 ? 1 : 0 );

                buff[ 0  ] = 's';
                buff[ 1  ] = 's';
                buff[ 2  ] = 0;
                buff[ 3  ] = 0;
                buff[ 4  ] = 0;
                buff[ 5  ] = 7 + bytes_cnt;
                buff[ 6  ] = 0; //nodes[ i ]->number;
                buff[ 7  ] = 0x0F;
                buff[ 8  ] = 0;
                buff[ 9  ] = 0;
                buff[ 10 ] = ( unsigned char ) nd->DO_cnt >> 8;
                buff[ 11 ] = ( unsigned char ) nd->DO_cnt & 0xFF;
                buff[ 12 ] = bytes_cnt;

                for ( u_int j = 0, idx = 0; j < bytes_cnt; j++ )
                    {
                    u_char b = 0;
                    for ( u_int k = 0; k < 8; k++ )
                        {
                        if ( idx < nd->DO_cnt )
                            {
                            b = b | ( nd->DO_[ idx ] & 1 ) << k;
                            idx++;
                            }
                        }
                    buff[ j + 13 ] = b;
                    }

                if ( e_communicate( nd, bytes_cnt + 13, 12 ) == 0 )
                    {
                    if ( buff[ 7 ] == 0x0F )
                        {
                        memcpy( nd->DO, nd->DO_, nd->DO_cnt );
                        }
                    }// if ( e_communicate( nd, bytes_cnt + 13, 12 ) > 0 )
                else
                    {
#ifdef DEBUG
                    //Print("\nWrite DO:Wago returned error...\n");
#endif // DEBUG
                    }
                }// if ( nd->DO_cnt > 0 )

            if ( nd->AO_cnt > 0 )
                {
                u_int bytes_cnt = nd->AO_size;

                buff[ 0  ] = 's';
                buff[ 1  ] = 's';
                buff[ 2  ] = 0;
                buff[ 3  ] = 0;
                buff[ 4  ] = 0;
                buff[ 5  ] = 7 + bytes_cnt;
                buff[ 6  ] = 0; //nodes[ i ]->number;
                buff[ 7  ] = 0x10;
                buff[ 8  ] = 0;
                buff[ 9  ] = 0;
                buff[ 10 ] = bytes_cnt / 2 >> 8;
                buff[ 11 ] = bytes_cnt / 2 & 0xFF;
                buff[ 12 ] = bytes_cnt;

                int l = 0;
                for ( unsigned int idx = 0; idx < nd->AO_cnt; idx++ )
                    {
                    
                    buff[ 13 + l] = (unsigned char)(( nd->AO_[ idx ] >> 8 ) & 0xFF);
                    buff[ 13 + l + 1] = (unsigned char)(nd->AO_[ idx ] & 0xFF);
                    
                    if ( nd->AI_types[ idx ] == 638 )
                        {
                        buff[ 13 + l + 2 ] = 0;
                        buff[ 13 + l + 3 ] = 0;
                        l += 2;                        
                        }
                    l += 2;
                    }

                if ( e_communicate( nd, bytes_cnt + 13, 12 ) == 0 )
                    {
                    if ( buff[ 7 ] == 0x10 )
                        {
                        memcpy( nd->AO, nd->AO_, sizeof( nd->AO ) );
                        }
                    }// if ( e_communicate( nd, 2 * bytes_cnt + 13, 12 ) == 0 )
                else
                    {
#ifdef DEBUG
                    Print("\nWrite AO:Wago returned error...\n");
#endif // DEBUG
                    }
                }// if ( nd->AO_cnt > 0 )

            }// if ( nd->type == wago_node::T_750_341 || ...
        }// for ( u_int i = 0; i < nodes_count; i++ )

    return 0;
    }
//-----------------------------------------------------------------------------
int wago_manager_linux::e_communicate( wago_node *node, int bytes_to_send,
    int bytes_to_receive )
    {
    // �������� ����� � ����� Wago.
    if( get_sec() - node->last_poll_time > wago_node::C_MAX_WAIT_TIME )
        {
        if( false == node->is_set_err )
            {
            node->is_set_err = true;
            PAC_critical_errors_manager::get_instance( )->set_global_error(
                PAC_critical_errors_manager::AC_NO_CONNECTION,
                PAC_critical_errors_manager::AS_WAGO, node->number );
            }
        }
    else
        {
        if( node->is_set_err )
            {
            node->is_set_err = false;
            PAC_critical_errors_manager::get_instance( )->reset_global_error(
                PAC_critical_errors_manager::PAC_critical_errors_manager::AC_NO_CONNECTION,
                PAC_critical_errors_manager::AS_WAGO, node->number );
            }
        }
    // �������� ����� � ����� Wago.-!>

    // ������������� �������� ����������, ��� �������������.
    if( node->state != wago_node::ST_OK )
        {
        if ( get_delta_millisec( node->last_init_time ) < node->delay_time )
            {
            return 1;
            }

        net_init( node );
        if( node->state != wago_node::ST_OK )
            {
            node->last_init_time = get_millisec();
            if ( node->delay_time < wago_node::C_MAX_DELAY )
                {
                node->delay_time += 10000 + node->number * 200;
                }

            return -100;
            }
        }
    // ������������� �������� ����������, ��� �������������.-!>

    node->delay_time = 0;

    // ������� ������.
    int res = send( node->sock, buff, bytes_to_send, 0 );
    if( res < 0 )
        {
#ifdef DEBUG
        if ( node->is_set_err == false )
            {
            perror( "wago_manager::e_communicate(...) send error" );
            }
#endif
        disconnect( node );

        return -101;
        }

    // ��������� ������.
    res = tcp_communicator_linux::recvtimeout( node->sock, buff,
        bytes_to_receive, 0, wago_node::C_RCV_TIMEOUT_US, node->ip_address );

    if( res <= 0 ) /* read error */
        {
#ifdef DEBUG
        if ( node->is_set_err == false )
            {
            perror( "wago_manager::e_communicate(...) recvtimeout error\n" );
            }
#endif
        disconnect( node );

        return -102;
        }

    node->last_poll_time = get_sec();

    return 0;
    }
//-----------------------------------------------------------------------------
int wago_manager_linux::read_inputs()
        {
        if ( 0 == nodes_count ) return 0;

        for ( u_int i = 0; i < nodes_count; i++ )
            {
            wago_node *nd = nodes[ i ];

            if ( nd->type == wago_node::T_750_XXX_ETHERNET ) // Ethernet Wago nodes.                
                {
                if ( !nd->is_active )
                    {
                    continue;
                    }

                if ( nd->DI_cnt > 0 )
                    {
                    buff[ 0  ] = 's';
                    buff[ 1  ] = 's';
                    buff[ 2  ] = 0;
                    buff[ 3  ] = 0;
                    buff[ 4  ] = 0;
                    buff[ 5  ] = 6;
                    buff[ 6  ] = 0; //nd->number;
                    buff[ 7  ] = 0x02;
                    buff[ 8  ] = 0;
                    buff[ 9  ] = 0;
                    buff[ 10 ] = ( unsigned char ) nd->DI_cnt >> 8;
                    buff[ 11 ] = ( unsigned char ) nd->DI_cnt & 0xFF;

                    u_int bytes_cnt = nd->DI_cnt / 8 + ( nd->DI_cnt % 8 > 0 ? 1 : 0 );

                    if ( e_communicate( nd, 12, bytes_cnt + 9 ) == 0 )
                        {
                        if ( buff[ 7 ] == 0x02 && buff[ 8 ] == bytes_cnt )
                            {
                            for ( u_int j = 0, idx = 0; j < bytes_cnt; j++ )
                                {
                                for ( int k = 0; k < 8; k++ )
                                    {
                                    if ( idx < nd->DI_cnt )
                                        {
                                        nd->DI[ idx ] =  ( buff[ j + 9 ] >> k ) & 1;
    #ifdef DEBUG_KBUS
                                        printf( "%d -> %d, ", idx, nd->DI[ idx ] );
    #endif // DEBUG_KBUS
                                        idx++;
                                        }
                                    }
                                }
    #ifdef DEBUG_KBUS
                            printf( "\n" );
    #endif // DEBUG_KBUS
                            }
                        else
                            {
    #ifdef DEBUG
                            Print("\nRead DI:Wago returned error...\n");
    #endif // DEBUG
                            }
                        }// if ( e_communicate( nd, 12, bytes_cnt + 9 ) == 0 )
                    }// if ( nd->DI_cnt > 0 )

                if ( nd->AI_cnt > 0 )
                    {
                    buff[ 0  ] = 's';
                    buff[ 1  ] = 's';
                    buff[ 2  ] = 0;
                    buff[ 3  ] = 0;
                    buff[ 4  ] = 0;
                    buff[ 5  ] = 6;
                    buff[ 6  ] = 0; //nd->number;
                    buff[ 7  ] = 0x04;
                    buff[ 8  ] = 0;
                    buff[ 9  ] = 0;

                    u_int bytes_cnt = nd->AI_size;

                    buff[ 10 ] = ( unsigned char ) bytes_cnt / 2 >> 8;
                    buff[ 11 ] = ( unsigned char ) bytes_cnt / 2 & 0xFF;

                    if ( e_communicate( nd, 12, bytes_cnt + 9 ) == 0 )
                        {
                        if ( buff[ 7 ] == 0x04 && buff[ 8 ] == bytes_cnt )
                            {
                            int idx = 0;
                            for ( unsigned int l = 0; l < nd->AI_cnt; l++ )
                                {
                                switch( nd->AI_types[ l ] )
                                    {
                                    case 638:
                                        nd->AI[ l ] = 256 * buff[ 9 + idx + 2 ] + 
                                            buff[ 9 + idx + 3 ];
                                        idx += 4;
                                        break;
                                        
                                    default:
                                        nd->AI[ l ] = 256 * buff[ 9 + idx ] + 
                                            buff[ 9 + idx + 1 ];
                                        idx += 2;
                                        break;
                                    }                                
                                }
                            }
                        else
                            {
    #ifdef DEBUG
                            Print("\nRead AI:Wago returned error. Node %d.\n",
                                nd->number );
                            Print( "bytes_cnt = %d, %d %d \n",
                                ( int ) buff[ 7 ], ( int ) buff[ 8 ], bytes_cnt );
    #endif // DEBUG
                            }
                        }
                    }// if ( nd->AI_cnt > 0 )

                }// if ( nd->type == wago_node::T_750_341 || ...
            }// for ( u_int i = 0; i < nodes_count; i++ )

        return 0;
        }
//-----------------------------------------------------------------------------
void wago_manager_linux::disconnect( wago_node *node )
    {
    shutdown( node->sock, SHUT_RDWR );
    close( node->sock );
    node->sock = 0;

    node->state = wago_node::ST_NO_CONNECT;
    }
//-----------------------------------------------------------------------------
wago_manager_linux::wago_manager_linux()
    {
    }
//-----------------------------------------------------------------------------
wago_manager_linux::~wago_manager_linux()
    {
    }
//-----------------------------------------------------------------------------
