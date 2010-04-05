#include <string.h>
#include <stdio.h>

#include "g_device.h"

#ifdef WIN32
#include "bug_log.h"
#include "pc_device.h"

#pragma warning( disable : 4996 ) //sprintf to sprintf_s
#endif // WIN32

#ifdef PAC

#if defined I7186_E || defined I7188_E || defined I7188
#include "utils.h"
#include "PAC_err.h"

extern PAC_critical_errors_manager *g_pac_critical_errors;
#endif // defined I7186_E || defined I7188_E || defined I7188

extern device_communicator  *g_dev_cmmctr;
unsigned int        device_communicator::dev_cnt;
i_complex_device ** device_communicator::dev;

#endif // PAC

int complex_device::MAX_NAME_LENGTH = 20;

#ifdef WIN32
u_int_2 G_PROTOCOL_VERSION = 3;
#else 

#ifdef USE_SIMPLE_DEV_ERRORS
u_int_2 G_PROTOCOL_VERSION = 3;
#else
u_int_2 G_PROTOCOL_VERSION = 2;
#endif // USE_SIMPLE_DEV_ERRORS

#endif // WIN32

//-----------------------------------------------------------------------------
void print_str( char *err_str, char is_need_CR )
    {    
#ifdef WIN32
    bug_log::add_msg( "System", "", err_str );
#else
    Print( "%s", err_str  );
    if ( is_need_CR )
        {
        Print( "\n" );
        }
#endif // WIN32
    }
//-----------------------------------------------------------------------------
complex_device::complex_device(): sub_dev( 0 ),
        n( 0 ),
        sub_dev_cnt( 0 ),
        type( 0 )        
    {  
    name = new char[ MAX_NAME_LENGTH ];
#ifdef WIN32
    strcpy_s( name, MAX_NAME_LENGTH - 1, "?" );    
#else
    strcpy( name, "?" );   
#endif
    }
//-----------------------------------------------------------------------------
complex_device::complex_device( u_int_4 n, char *new_name, 
                               u_int_2 new_subdev_cnt, char type ): n( n ),
        sub_dev_cnt( new_subdev_cnt ),
        type( type )
    {           
    name = new char[ MAX_NAME_LENGTH ];
#ifdef WIN32
    strcpy_s( name, MAX_NAME_LENGTH - 1, new_name );    
#else
    strcpy( name, new_name );   
#endif

    if ( sub_dev_cnt ) sub_dev = new i_simple_device*[ sub_dev_cnt ];
    else sub_dev = 0;
    }
//-----------------------------------------------------------------------------
#ifdef WIN32
complex_device::complex_device( u_int_4 n,
                               char *new_name, 
                               u_int_2 new_subdev_cnt, 
                               char type, 
                               i_complex_device *owner_compl_dev ): type( type ), 
                               n( n ), 
                               sub_dev_cnt( new_subdev_cnt )
    {
    name = new char[ MAX_NAME_LENGTH ];
    strcpy_s( name, MAX_NAME_LENGTH - 1, new_name );    

    if ( sub_dev_cnt ) sub_dev = new i_simple_device*[ sub_dev_cnt ];
    else sub_dev = 0;

    compl_dev = owner_compl_dev;
    }
//-----------------------------------------------------------------------------
complex_device::~complex_device()
    {
    if ( name ) 
        {
        delete [] name;
        name = 0;
        }
    if ( sub_dev )
        {
        delete [] sub_dev;
        sub_dev = 0;
        }
    }
#endif //WIN32
//-----------------------------------------------------------------------------
char complex_device::get_type() const
    {
    return type;
    }
//-----------------------------------------------------------------------------
u_int_4 complex_device::get_n() const
    {
    return n;
    }
//-----------------------------------------------------------------------------
const char* complex_device::get_name() const
    {
    return name;
    }
//-----------------------------------------------------------------------------
u_int_4 complex_device::get_subdev_quantity() const
    {
    return sub_dev_cnt;
    }
//-----------------------------------------------------------------------------
i_save_device*  complex_device::get_save_dev( u_int_4 idx ) const
    {
#if defined DEBUG && defined WIN32
    if ( idx >= sub_dev_cnt )
        {
        _DebugBreak();
        }
#endif // DEBUG

    return sub_dev[ idx ];
    }
//-----------------------------------------------------------------------------
// ������ ������ (buff) � ��������� ����:
//    1 ����  - ���;                                    (1)
//            0 - ������� ����������;
//            1 - ������ ��������� � ������������ ������ 1 ����;
//            2 - ������ ��������� � ������������ ������ 4 �����;
//    4 ����� - �����;                                  (2)
//    1 ����  - ����� ����� ������ ����������;          (3)
//    � ����  - ��� ������ ����������;                  (4)
//    4 ����� - ���������� ������������;                (5)
//    �����   - ������ ������������.
int  complex_device::save_device( char *buff )
    {
    int idx = 0;

    buff[ 0 ] = get_type();                             //(1)
    buff++;
    idx++;
    ( ( u_int_4* ) buff )[ 0 ] = get_n();               //(2)
    buff += 4;
    idx += 4;
    buff[ 0 ] = ( unsigned char ) strlen( get_name() ); //(3)
    buff++;
    idx++;
#ifndef WIN32
    strcpy( buff, get_name() );                         //(4)
#else
    strcpy_s( buff, 255, get_name() );                  //(4)
#endif
    buff += strlen( get_name() ) + 1;
    idx += strlen( get_name() ) + 1;
    ( ( u_int_4* ) buff )[ 0 ] = get_subdev_quantity(); //(5)
    buff += 4;
    idx += 4;

    for ( u_int_4 i = 0; i < get_subdev_quantity(); i++ )
        {
        u_int_4 size = get_save_dev( i )->save_device( buff );
        buff += size;
        idx += size;        
        }
    return idx;
    }
//-----------------------------------------------------------------------------
// ������ ������ (buff) � ��������� ����:
//  4 ����� - ����� ����������;                         (1)
//  4 ����� - ���������� ������������;                  (2)
//  �����   - ������ ������� �������������.
int  complex_device::save_state( char *buff )
    {
    ( ( u_int_4* ) buff )[ 0 ] = get_n();               //(1)
    ( ( u_int_4* ) buff )[ 1 ] = get_subdev_quantity(); //(2)
    unsigned int answer_size = 8;

    for ( unsigned int i = 0; i < get_subdev_quantity(); i++ )
        {
        answer_size += get_save_dev( i )->save_state( buff + answer_size );
        }
    return answer_size;
    }
//-----------------------------------------------------------------------------
int  complex_device::set_type( char new_type )
    {
    type = new_type;
    return 0;
    }
//-----------------------------------------------------------------------------
int  complex_device::set_n( u_int_4 new_n )
    {
    n = new_n;
    return 0;
    }
//-----------------------------------------------------------------------------
int  complex_device::set_name( char * new_name )
    {
    if ( name )         
        {
#ifdef WIN32
        strcpy_s( name, MAX_NAME_LENGTH - 1, new_name );    
#else
        strcpy( name, new_name );   
#endif
        }
    return 0;
    }
//-----------------------------------------------------------------------------
int complex_device::set_subdev_quantity( u_int_4 new_dev_cnt )
    {
    sub_dev_cnt = new_dev_cnt;
    sub_dev = new i_simple_device*[ new_dev_cnt ];

    return 0;
    }
//-----------------------------------------------------------------------------
i_load_device* complex_device::get_load_dev( u_int_4 idx )
    {
#if defined DEBUG && defined WIN32
    if ( idx >= sub_dev_cnt )
        {
        _DebugBreak();
        }
#endif // DEBUG

    return sub_dev[ idx ];
    }
//-----------------------------------------------------------------------------
// ������ ������ (buff) � ��������� ����:
//    1 ����  - ���;                                    (1)
//            0 - ������� ����������;
//            1 - ������ ��������� � ������������ ������ 1 ����;
//            2 - ������ ��������� � ������������ ������ 4 �����;
//    4 ����� - �����;                                  (2)
//    1 ����  - ����� ����� ������ ����������;          (3)
//    � ����  - ��� ������ ����������;                  (4)
//    4 ����� - ���������� ������������;                (5)
//    �����   - ������ ������������.
int  complex_device::load_device( char *buff )
    {
#ifdef WIN32
    int idx = 0;
    set_type( buff++[ 0 ] );                            //(1)
    idx++;
    set_n( ( ( u_int_4* ) buff )[ 0 ] );                //(2)
    buff += 4;
    idx += 4;
    uchar name_len = buff++[ 0 ];                       //(3)
    idx++;

    if ( buff[ name_len ] != 0 )
        {            
        sprintf_s( bug_log::msg, bug_log::msg_size, 
            "i_complex_device::load_device( char *buff ) - �������� ��� ����������!" );
        //bug_log::println_msg();
        bug_log::add_warning_msg( "Driver", "" );
        return -1;
        }

    char *new_name = new char[ name_len + 1 ];

    strcpy_s( new_name, MAX_NAME_LENGTH, buff );
    buff += name_len + 1;
    idx += name_len + 1;
    set_name( new_name );

    u_int_4 subdev_quantity = ( ( u_int_4* ) buff )[ 0 ];
    set_subdev_quantity( subdev_quantity );             //(5)
    buff += 4;
    idx += 4;

    int res = 0;
    for ( u_int_4 i = 0; i < subdev_quantity; i++ )
        {        
        switch ( type )
            {
            case COMPLEX_DEV:
                sub_dev[ i ] = new complex_device( i + 1, "?", 0, GROUP_DEV_BYTE, this );
                res = get_load_dev( i )->load_device( buff );
                if ( res < 0 )
                    {
                    return res;
                    }

                buff += res;
                idx += res; 

                sub_dev[ i ]->set_idx( i );
                break;

            case GROUP_DEV_BYTE:
            case GROUP_DEV_LONG:
            case GROUP_DEV_FLOAT:
                try
                    {
                    sub_dev[ i ] = new virtual_device( this );
                    }
                catch ( std::bad_alloc exception )
                    {                        
                    bug_log::add_warning_msg( "Driver", 
                        "complex_device::load_device - �� ������� �������� ������ ��� �������� �������!" );
                    return -2;
                    }                

                res = get_load_dev( i )->load_device( buff );                
                if ( res < 0 )
                    {
                    return res;
                    }

                buff += res;
                idx += res;    

                sub_dev[ i ]->set_idx( i );
                break;

            case ARRAY_DEV_BYTE:
            case ARRAY_DEV_FLOAT:
            case ARRAY_DEV_LONG:
            case ARRAY_DEV_UINT:
            case ARRAY_DEV_INT: 
            case ARRAY_DEV_STR:
                sub_dev[ i ] = new virtual_device( this, i + 1 );
                sub_dev[ i ]->set_idx( i );
                break;

            default:                    
                sprintf_s( bug_log::msg, bug_log::msg_size, 
                    "i_complex_device::load_device( char *buff ) - �������� ��� ���������� = %d!",
                    type );
                bug_log::add_warning_msg( "Driver", "" );
#ifdef _DEBUG
                _DebugBreak();
#endif
                return -1;
            }   

        //sprintf_s( bug_log::msg, bug_log::msg_size, "complex_device - load_device \"%s\"[%d] of %d", 
        //    new_name, i, subdev_quantity );
        //bug_log::println_msg();
        //Sleep( 100 );

        }
    return idx;
#else
    buff++;
    return 0;
#endif
    }
//-----------------------------------------------------------------------------
// ������ ������ (buff) � ��������� ����:
//  4 ����� - ����� ����������;                         (1)
//  4 ����� - ���������� ������������;                  (2)
//  �����   - ������ ������� �������������.
int  complex_device::load_state( char *buff  )
    {
#ifdef WIN32
    u_int_4 rec_id = ( ( u_int_4* ) buff )[ 0 ];        //(1) 
    u_int_4 rec_groups_cnt = ( ( u_int_4* ) buff )[ 1 ];//(2) 
    buff += 8;
    u_int_4 idx = 8;

    if ( rec_id != get_n() ||
        rec_groups_cnt != get_subdev_quantity() )
        {
        char err_str[ 600 ];
        sprintf( err_str, "i_complex_device::load_state( char *buff ) - Error! rec_id = %lu, rec_groups_cnt=%lu, get_n() = %lu, get_subdev_quantity() = %lu!\n", 
            rec_id, rec_groups_cnt, get_n(), get_subdev_quantity() );
        print_str( err_str );
        return -1;
        }

    //if ( !get_subdev_quantity() ) return idx;

    int res = 0;
    for ( unsigned int i = 0; i < get_subdev_quantity(); i++ )
        {      
        res = get_load_dev( i )->load_state( buff );
        if ( res < 0 )
            {
            return res;
            }
        buff += res;
        idx += res;
        }

    return idx;

#else
    buff++;
    return 0;
#endif // WIN32     
    }    
//-----------------------------------------------------------------------------
void complex_device::print() const
    {
    static char indent[ 40 ] = "";
    static int start_pos = 0;
    memset( indent + start_pos, ' ', 4 );
    start_pos += 4;
    indent[ start_pos ] = 0;

    char tmp_str[ 200 ];  

    if ( start_pos > 4 )
        {
        sprintf( tmp_str, "\"%s\"[ %3lu ], \ttype = %d", name,
            ( unsigned long int  ) sub_dev_cnt, type );
        }
    else
        {
        sprintf( tmp_str, "\"%s%lu\"[ %3lu ], \ttype = %d", 
                name, ( unsigned long int  ) get_n(),
                ( unsigned long int  ) sub_dev_cnt, type );
        }

    print_str( tmp_str );  

    for ( unsigned int i = 0; i < sub_dev_cnt; i++ ) 
        {
        sprintf( tmp_str, "%s[ %3d ] ", indent, i );
        print_str( tmp_str, 0 );
        sub_dev[ i ]->print();        
        }

    start_pos -= 4;
    indent[ start_pos ] = 0;    
    }
//-----------------------------------------------------------------------------
i_simple_device* complex_device::get_sub_dev( u_int_4 id ) const
    {
    //-�������� �����.
    int l = 0;
    int u = sub_dev_cnt - 1;
    int i;
    while ( l <= u ) 
        {
        i = ( l + u ) / 2;

        if ( sub_dev[ i ]->get_n() == id  )
            {
            return sub_dev[ i ];
            }

        if ( id > sub_dev[ i ]->get_n() ) 
            {
            l = i + 1;
            } 
        else 
            {
            u = i - 1;
            }
        }
    //-�������� �����.-!>

    return 0;
    }
//-----------------------------------------------------------------------------
//���������� �������, ���������� � ������.
//��������� ������ buff:
//    4 ����� - ������ ���������� � ������� ���������;
//    � ����  - ������ ������� ����������.
int complex_device::parse_cmd( char *buff  ) 
    {
    u_int_4 dev_n = ( ( u_int_4* )( buff ) )[ 0 ];

#if defined DEBUG && defined WIN32
    if ( dev_n >= sub_dev_cnt )
        {
        _DebugBreak();
        }
#endif // DEBUG

    return 4 + sub_dev[ dev_n ]->parse_cmd( buff + 4 );
    }
//-----------------------------------------------------------------------------
u_int_4 complex_device::get_idx()
    {
    return idx;
    }
//-----------------------------------------------------------------------------
void complex_device::set_idx( u_int_4 new_idx )
    {
    idx = new_idx;
    }
//-----------------------------------------------------------------------------
i_complex_device* complex_device::get_sub_complex_dev( char *sub_dev_name ) const
    {
    if ( !sub_dev ) return 0;
    if ( GROUP_DEV_BYTE == type || GROUP_DEV_FLOAT == type ) return 0;

    for ( unsigned int i = 0; i < sub_dev_cnt; i++ ) 
        {
        if ( strcmp( ( ( i_complex_device* )sub_dev[ i ] )->get_name(), 
            sub_dev_name ) == 0 )
            {
            return ( i_complex_device* ) sub_dev[ i ];
            }
        }

    return 0;
    }
//-----------------------------------------------------------------------------
// ������ ������ (buff) � ��������� ����:
//  2 ����� - ���������� ���������;                                 (1)
//  2 ����� - ������ ���������� � ������� ���������;                (2)
//  �����   - ������ ������� �������������.                         (3)
int complex_device::load_changed_state( char *buff )
    {
#ifdef WIN32 
    u_int_2 rec_groups_cnt = ( ( u_int_2* ) buff )[ 0 ];            //1 
    buff += 2;
    int idx = 2;

    if ( !get_subdev_quantity() ) return -1;

    int res = 0;
    for ( u_int_2 i = 0; i < rec_groups_cnt; i++ )
        {      
        u_int_2 dev_idx = ( ( u_int_2* ) buff )[ 0 ];               //2
        buff += 2;
        idx += 2;

        res = get_load_dev( dev_idx )->load_changed_state( buff );  //3
        if ( res < 0 )
            {
            return -1;
            }        
        buff += res;
        idx += res;
        }

    return idx;

#else
    buff++;
    return 0;
#endif // WIN32
    }
//-----------------------------------------------------------------------------
// ������ ������ (buff) � ��������� ����:          
//  2 ����� - ���������� ���������, ���������� ���� ��������;   (1)
//  2 ����� - ����� ���������� � ������� ���������.             (2)
//  �����   - ��� ��������� ������                              (3)
//�������� ������ ������.                                       (4)
//����������� �� 1 ���������� ���������, ���������� ����
//���������.                                                    (5)
//���� �� ���������, ���������� ���� ���������, ���������� 0.   (6)
int complex_device::save_changed_state( char *buff )
    {
    u_int_2 *changed_sub_dev_cnt = ( u_int_2* ) buff;           //1
    *changed_sub_dev_cnt = 0;
    int answer_size = 2;

    for ( u_int_2 i = 0; i < get_subdev_quantity(); i++ )
        {
        ( ( u_int_2* ) ( buff + answer_size ) )[ 0 ] = i;       //2        
        int res = get_save_dev( i )->save_changed_state( 
            buff + answer_size + 2 );                           //3
        if ( res ) 
            {            
            answer_size = answer_size + 2 + res;                //4
            ( *changed_sub_dev_cnt )++;                         //5
            }
        }
    if ( 2 == answer_size )                                     //6             
        {
        return 0;
        }

    return answer_size;    
    }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
device_communicator::device_communicator()
    {  
    dev_cnt = 0;
    dev = 0;

#ifdef PAC
    dev = new i_complex_device*[ 40 ];
    for ( int i = 0; i < 40; i++ )
        {
        dev[ i ] = 0;
        }
#endif // PAC
    }
//-----------------------------------------------------------------------------
i_complex_device* device_communicator::get_group( char* dev_name, u_int_4 dev_n, 
                                                 char* name ) const
    {
    if ( !dev ) return 0;

    for ( unsigned int i = 0; i < dev_cnt; i++ ) 
        {
        if ( strcmp( dev[ i ]->get_name(), dev_name ) == 0 && 
            dev[ i ]->get_n() == dev_n )
            {
            return ( ( complex_device* ) dev[ i ] )->get_sub_complex_dev( name );
            }
        }

    return 0;
    }
//-----------------------------------------------------------------------------
int device_communicator::load_state( char *buff  )
    {
    //��������� ���������� �� ����������� ������:
    // 4 ����� - ���������� ���������;
    // ����� ���� ������ ������ ������.

    //int rec_groups_cnt = ( ( u_int_4* ) buff )[ 0 ]; 
    buff += 4;

    if ( !dev ) return -1;
    int res = 0;

    for ( unsigned int i = 0; i < dev_cnt; i++ )
        {       
        res = dev[ i ]->load_state( buff );  
        if ( res < 0 )
            {
            return res;
            }
        buff += res;
        }

    return 0;
    }
//-----------------------------------------------------------------------------
int device_communicator::load_device( char *buff  ) 
    {
    //��������� ���������� �� ����������� ������:
    // 4 ����� - ���������� ����� ���������;
    // ����� ���� ������ ������ ������.
#ifdef WIN32
    dev_cnt = ( ( u_int_4* ) buff )[ 0 ]; 
    buff += 4;
    if ( dev )
        {        
        bug_log::add_msg( "Driver", "", "������� ���������� ���������� PAC!" );
        delete[] dev;
        dev = 0;
        }
    try
        {
        dev = new i_complex_device*[ dev_cnt ];  
        }
    catch ( std::bad_alloc exception )
        {                        
        bug_log::add_warning_msg( "Driver", "",
            "device_communicator::load_device(...) - �� ������� �������� ������!" );
        return -2;
        }
#else
    dev = new i_complex_device*[ dev_cnt ];  
#endif

    int res = 0;
    for ( unsigned int i = 0; i < dev_cnt; i++ )
        {
#ifdef WIN32
        try
            {
            dev[ i ] = new complex_device(); 
            }
        catch ( std::bad_alloc exception )
            {                                    
            bug_log::add_warning_msg( "Driver", "",
                "device_communicator::load_device(...) - �� ������� �������� ������!" );
            return -2;
            }
#else
        dev[ i ] = new complex_device(); 
#endif

        dev[ i ]->set_idx( i ); 

        res = dev[ i ]->load_device( buff );
        if ( res < 0 )
            {
            return res;
            }

        buff += res;
        }

    return 0;
    }
//-----------------------------------------------------------------------------
void device_communicator::print() const
    {  
    char tmp_str[ 200 ];

    sprintf( tmp_str, "\nDevice communicator. Dev count = %u.", dev_cnt );
    print_str( tmp_str );

    if ( !dev_cnt ) return;

    for ( unsigned int i = 0; i < dev_cnt; i++ ) 
        {
        sprintf( tmp_str, "[ %3d ] ", i );
        print_str( tmp_str, 0 );
        dev[ i ]->print();
        } 
    sprintf( tmp_str, "Done." );
    print_str( tmp_str );
    }
//-----------------------------------------------------------------------------
int device_communicator::load_changed_state( char *buff )
    {
    // ��������� ���������� �� ����������� ������:
    //2 ����� - ���������� ���������;                           (1)
    //��� ������� ����������:                                   (2)
    //����� ���� ����� ���������� � ������� ���������,          (3)
    //����� ������ ����������.                                  (4)                               

    u_int_2 rec_dev_cnt = ( ( u_int_2* ) buff )[ 0 ];           //1 
    buff += sizeof( rec_dev_cnt );

    if ( !dev ) return -1;
    int res = 0;

    for ( u_int_2 i = 0; i < rec_dev_cnt; i++ )                 //2
        {     
        u_int_2 device_idx = ( ( u_int_2* ) buff )[ 0 ];        //3
        if ( device_idx > dev_cnt)
            {
#ifdef WIN32
            sprintf_s( bug_log::msg, bug_log::msg_size,
                "device_communicator::load_changed_state(...) - ����� ���������� [ %d ] ������ ���������� ��������� [ %d ]!",
                device_idx, dev_cnt );
            bug_log::add_warning_msg( "Driver", "" );
#endif //WIN32

#ifdef _DEBUG
            _DebugBreak();
#endif        
            return -1;
            }

        buff += sizeof( device_idx );
        res = dev[ device_idx ]->load_changed_state( buff );    //4       
        if ( res < 0 )
            {
            return res;
            }
        buff += res;            
        }

    return 0;
    }
//-----------------------------------------------------------------------------
#ifdef PAC

#ifdef USE_SIMPLE_DEV_ERRORS
#include "errors.h"

extern dev_errors_manager *g_dev_errors_manager; 
#endif // USE_SIMPLE_DEV_ERRORS

long device_communicator::write_devices_states_service( long len, 
                                                       u_char *data,
                                                       u_char *outdata )
    {
    if ( len < 1 ) return 0;

    unsigned int i;
    unsigned long answer_size = 0;

    //  [3/25/2009 id]
#ifdef DEBUG_DEV_CMCTR
    //unsigned long start_time = MyGetMS();
#endif // DEBUG_DEV_CMCTR             
    //

    switch ( data[ 0 ] )
        {
        case GET_INFO_ON_CONNECT:
#ifdef DEBUG_DEV_CMCTR
            Print( "G_PROTOCOL_VERSION = %u, host =[%s]\n", G_PROTOCOL_VERSION, 
                tcp_communicator::get_instance()->get_host_name() );
#endif // DEBUG_DEV_CMCTR

            ( ( u_int_2* ) outdata )[ 0 ] = G_PROTOCOL_VERSION; //������ ���������.
            outdata += 2;
            answer_size += 2;

            strcpy( ( char* ) outdata, 
                tcp_communicator::get_instance()->get_host_name() );
            answer_size += strlen( 
                tcp_communicator::get_instance()->get_host_name() ) + 1;
            return answer_size;

        case GET_DEVICES:
            static u_int_2 g_devices_request_id = 0;
            if ( 0 == g_devices_request_id )
                {
                g_devices_request_id = ( ( u_int_2* ) ( data + 1 ) )[ 0 ];                        
                }

            ( ( u_int_2* ) outdata )[ 0 ] = g_devices_request_id;
            answer_size += 2;
            ( ( u_int_4* ) ( outdata + answer_size ) )[ 0 ] = dev_cnt;
            answer_size += 4;

            for ( i = 0; i < dev_cnt; i++ )
                {
                answer_size += dev[ i ]->save_device( ( char* ) outdata + 
                    answer_size );                
                }      
#ifdef DEBUG_DEV_CMCTR
            Print( "Devices size = %lu, g_devices_request_id = %d\n", 
                answer_size, 
                g_devices_request_id );

            //Print( "Operation time = %lu\n", MyGetMS() - start_time );
#endif // DEBUG_DEV_CMCTR
            return answer_size;

        case GET_DEVICES_STATES:
            ( ( u_int_2* ) outdata )[ 0 ] = g_devices_request_id;
            answer_size += 2;
            ( ( u_int_4* ) ( outdata + answer_size ) )[ 0 ] = dev_cnt;
            answer_size += 4;

            for ( i = 0; i < dev_cnt; i++ )
                {
                answer_size += dev[ i ]->save_state( ( char* ) outdata + 
                    answer_size );
                }
#ifdef DEBUG_DEV_CMCTR
            Print( "Devices states size = %lu, g_devices_request_id = %d\n", 
                answer_size, g_devices_request_id );

            //Print( "Operation time = %lu\n", MyGetMS() - start_time );
#endif // DEBUG_DEV_CMCTR
            return answer_size;

        case GET_DEVICES_CHANGED_STATES:
            ( ( u_int_2* ) outdata )[ 0 ] = g_devices_request_id;
            answer_size += 2;
            ( ( u_int_2* ) ( outdata + answer_size ) )[ 0 ] = 0;
            answer_size += 2;

            for ( i = 0; i < dev_cnt; i++ )
                {
                u_int_2 res = dev[ i ]->save_changed_state( ( char* ) outdata +
                    answer_size + 2 );
                if ( res )
                    {
                    ( ( u_int_2* ) ( ( char* ) outdata + 
                        answer_size ) )[ 0 ] = i;
                    answer_size += 2;
                    answer_size += res;
                    ( ( u_int_2* ) outdata )[ 1 ]++;
                    }                
                }
#ifdef DEBUG_DEV_CMCTR
            Print( "\nChanged states size = %d\n", answer_size );

            //Print( "Operation time = %lu\n", MyGetMS() - start_time );
#endif // DEBUG_DEV_CMCTR
            return answer_size;

        case EXEC_DEVICE_CMD:
#ifdef DEBUG_DEV_CMCTR
            Print( "\nEXEC_DEVICE_CMD\n" );
            Print( "unsigned long buff[1] - %lu; ", 
                ( ( u_int_4* ) ( data + 1 ) )[ 1 ] );
            Print( "[2] - %lu; ", ( ( u_int_4* ) ( data + 1 ) )[ 2 ] );
            Print( "[3] - %lu; ", ( ( u_int_4* ) ( data + 1 ) )[ 3 ] );
            Print( "[4] - %lu\n", ( ( u_int_4* ) ( data + 1 ) )[ 4 ] );

            Print( "float         buff[1] - %f; ", 
                ( ( float* ) ( data + 1 ) )[ 1 ] );
            Print( "[2] - %f; ", ( ( float* ) ( data + 1 ) )[ 2 ] );
            Print( "[3] - %f; ", ( ( float* ) ( data + 1 ) )[ 3 ] );
            Print( "[4] - %f\n", ( ( float* ) ( data + 1 ) )[ 4 ] );
#endif // DEBUG_DEV_CMCTR

            dev[ ( ( u_int_4* )( ( char* ) data + 1 ) )[ 0 ] ]->parse_cmd( 
                ( char* ) data + 5 );
#ifdef DEBUG_DEV_CMCTR
            //Print( "Operation time = %lu\n", MyGetMS() - start_time );
#endif // DEBUG_DEV_CMCTR
            ( ( u_int_2* ) ( outdata + answer_size ) )[ 0 ] = 0; //���������� 0.
            answer_size += 2;
            return answer_size;

        case GET_PAC_ERRORS:            
#ifdef DEBUG_DEV_CMCTR
            Print( "GET_PAC_ERRORS\n" );
#endif

#if defined I7186_E
            if ( g_pac_critical_errors )
                {
                answer_size = g_pac_critical_errors->save_to_stream( outdata );
                }
#endif // defined I7186_E

#ifdef DEBUG_DEV_CMCTR
            Print( "Critical errors count = %d, answer size = %d\n",
                outdata[ 2 ], 
                answer_size );
#endif // DEBUG_DEV_CMCTR

#ifdef USE_SIMPLE_DEV_ERRORS
            answer_size += g_dev_errors_manager->save_to_stream( outdata + 
                answer_size );
#ifdef DEBUG_DEV_CMCTR
            Print( "Simple devices errors count = %d, answer size = %d\n", 
                ( outdata + answer_size )[ 4 ], answer_size );
#endif // DEBUG_DEV_CMCTR
#endif // USE_SIMPLE_DEV_ERRORS

            return answer_size;

#ifdef USE_SIMPLE_DEV_ERRORS
        case SET_PAC_ERROR_CMD:            
            unsigned int count = *( ( u_int_2* ) ( data + 1 ) ); 

            unsigned int *uint_cmd = ( unsigned int* ) ( data + 1 + 2 );
            for ( i = 0; i < count; i++ )
                {
#ifdef DEBUG_DEV_CMCTR
                Print( "SET_PAC_ERROR_CMD" );
                Print( "cmd = %u, object_type = %u, object_number = %u, \
                       object_alarm_number = %u\n", uint_cmd[ 0 ],
                       uint_cmd[ 1 ], uint_cmd[ 2 ], uint_cmd[ 3 ]   );
#endif // DEBUG_DEV_CMCTR

                g_dev_errors_manager->set_cmd( uint_cmd[ 0 ], uint_cmd[ 1 ],
                    uint_cmd[ 2 ], uint_cmd[ 3 ] );

                uint_cmd += 4;
                }

            ( ( u_int_2* ) ( outdata + answer_size ) )[ 0 ] = 0; //���������� 0.
            answer_size += 2;

            return answer_size;
#endif // USE_SIMPLE_DEV_ERRORS
        }

    return answer_size;
    }                  
//-----------------------------------------------------------------------------
int device_communicator::add_device( i_complex_device *device )
    {
    dev[ dev_cnt++ ] = device;
    return 0;
    }
//-----------------------------------------------------------------------------
#endif //PAC
