#ifdef DRIVER
#include "stdafx.h"
#else
#include <errors.h>
#endif

#ifdef PAC
auto_smart_ptr < dev_errors_manager > dev_errors_manager::instance;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
base_error::base_error(): err_par( 1 ), error_state( AS_NORMAL )
    {
    }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
simple_error::simple_error( device* simple_device
                           ): base_error(),
                           simple_device( simple_device )
    {
    simple_device->set_err_par( &err_par );
    }
//-----------------------------------------------------------------------------
simple_error::~simple_error()
    {
    }
//-----------------------------------------------------------------------------
int simple_error::save_as_Lua_str( char *str, bool &is_new_state )
    {
    str[ 0 ] = 0;

    // �������� �������� ��������� ����������.
    switch ( simple_device->get_state() )
        {
        case -1:        // ���� ������.
        case -2:
        case -3:
        case -12:
        case -13:

            switch ( error_state )
                {
                case AS_ACCEPT:
                    break;

                case AS_ALARM:
                    break;

                case AS_NORMAL:
                case AS_RETURN:
                    error_state = AS_ALARM;
                    is_new_state = true;
                    break;
                }
            break;

        default:         // ��� ������ - ��� ��������� ���������.
            switch ( error_state )
                {
                case AS_NORMAL:
                    break;

                case AS_RETURN:
                    break;

                case AS_ACCEPT:
                    error_state = AS_NORMAL;
                    is_new_state = true;
                    break;

                case AS_ALARM:
                    error_state = AS_RETURN;
                    is_new_state = true;
                    break;
                }
            break;
        }
    // �������� �������� ��������� ����������.-!>

    if ( AS_ALARM == error_state || AS_ACCEPT == error_state ||
        AS_RETURN == error_state ) // ���� ������.
        {
        unsigned char alarm_params = err_par[ P_PARAM_N ];

        sprintf( str + strlen( str ), "\t%s\n", "{" );

        sprintf( str + strlen( str ), "\tdescription=\"%s - %s\",\n",
            simple_device->get_name(), simple_device->get_description() );

        sprintf( str + strlen( str ), "priority=%d%s", P_ALARM, "," );
        sprintf( str + strlen( str ), "state=%d,\n", error_state );
        sprintf( str + strlen( str ), "type=%d,\n", AT_SPECIAL );
        sprintf( str + strlen( str ), "group=\"%s\",\n", "�������" );

        sprintf( str + strlen( str ), "id_n=%d,\n", simple_device->get_serial_n() );
        sprintf( str + strlen( str ), "id_type=%d,\n", simple_device->get_type() );

        sprintf( str + strlen( str ), "suppress=%s\n",
            alarm_params && P_IS_SUPPRESS ? "true" : "false" );

        sprintf( str + strlen( str ), "},\n" );
        }

    return 0;
    }
//-----------------------------------------------------------------------------
void simple_error::print() const
    {
#ifdef DEBUG
    Print( "%s - state[ %3d ], par[ %d ]\n",
        simple_device->get_name(), error_state, err_par[ P_PARAM_N ] );
#endif // DEBUG
    }
//-----------------------------------------------------------------------------
unsigned char simple_error::get_object_type() const
    {
    return simple_device->get_type();
    }
//-----------------------------------------------------------------------------
unsigned int simple_error::get_object_n() const
    {
    return simple_device->get_serial_n();
    }
//-----------------------------------------------------------------------------
int simple_error::set_cmd( int cmd, int object_alarm_number )
    {
    int res = 0;
	int current_state = err_par[ P_PARAM_N ];

    switch ( cmd )
        {
        case C_CMD_SUPPRESS:
            {
            current_state |= P_IS_SUPPRESS;
            err_par[ P_PARAM_N ] = current_state;
            break;
            }

         case C_CMD_UNSET_SUPPRESS:
             {
             current_state |= P_IS_SUPPRESS;
             current_state ^= P_IS_SUPPRESS;

             err_par[ P_PARAM_N ] = current_state;
             break;
             }

        case C_CMD_ACCEPT:
            if ( AS_ALARM == error_state ||
                AS_RETURN == error_state )
                {
                if ( AS_RETURN == error_state )
                    {
                    error_state = AS_NORMAL;
                    }
                else
                    {
                    error_state = AS_ACCEPT;
                    }
                }
            else
                {
#ifdef DEBUG
                Print( "simple_error::set_cmd(...) - error state = %d, \
                       trying to set to ACCEPT!\n", error_state );
#endif // DEBUG
                res = 1;
                }
            break;

        default:
            res = 2;
            break;
        }

#ifdef DEBUG
    Print( "simple_error::set_cmd(...) - cmd = %d\n", cmd );
    print();
#endif // DEBUG

    return res;
    }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int tech_dev_error::save_as_Lua_str( char *str, bool &is_new_state )
    {
    str[ 0 ] = 0;
    static u_int prev_size = 0;

    if ( tech_dev->get_errors().size() != prev_size || was_set_cmd )
        {
        prev_size   = tech_dev->get_errors().size();
        was_set_cmd = false;

        is_new_state = true;
        }

    for ( u_int i = 0; i < tech_dev->get_errors().size(); i++ )
        {

        sprintf( str + strlen( str ), "\t%s\n", "{" );

        sprintf( str + strlen( str ), "\tdescription=\"%s\",\n",
            tech_dev->get_errors()[ i ]->msg );
        sprintf( str + strlen( str ), "\tgroup=\"%s\",\n",
            get_group( tech_dev->get_errors()[ i ]->type ) );

        sprintf( str + strlen( str ), "priority=%d%s",
            get_priority( tech_dev->get_errors()[ i ]->type ), "," );

        sprintf( str + strlen( str ), "state=%d,\n", AS_ALARM );
        sprintf( str + strlen( str ), "type=%d,\n", AT_SPECIAL );

        sprintf( str + strlen( str ), "id_n=%d,\n",
            tech_dev->get_number() );
        sprintf( str + strlen( str ), "id_object_alarm_number=%d,\n",
            tech_dev->get_errors()[ i ]->n );
        sprintf( str + strlen( str ), "id_type=%d,\n", get_object_type() );

        sprintf( str + strlen( str ), "suppress=false\n" );

        sprintf( str + strlen( str ), "},\n" );
        }

    return 0;
    }
//-----------------------------------------------------------------------------
int tech_dev_error::set_cmd( int cmd, int object_alarm_number )
    {
    for ( u_int i = 0; i < tech_dev->get_errors().size(); i++ )
        {
        if( tech_dev->get_errors()[ i ]->n == object_alarm_number )
            {
            tech_dev->get_errors().erase(
                tech_dev->get_errors().begin() + i );

#ifdef DEBUG
            Print( "Object %d set cmd %d [%d]!\n",
                object_alarm_number, cmd, object_alarm_number );
#endif // DEBUG
            was_set_cmd = true;
            return 0;
            }
        }
#ifdef DEBUG
    Print( "Object %d not found!\n", object_alarm_number );
#endif // DEBUG

    return 1;
    }
//-----------------------------------------------------------------------------
int tech_dev_error::get_priority( tech_object::ERR_MSG_TYPES err_type ) 
    {
    switch ( err_type )
        {
        case tech_object::ERR_CANT_ON:
        case tech_object::ERR_ON_WITH_ERRORS:
            return P_ANSWER;

        case tech_object::ERR_OFF:
        case tech_object::ERR_OFF_AND_ON:
        case tech_object::ERR_DURING_WORK:
            return P_MESSAGE;

        case tech_object::ERR_ALARM:
            return P_ALARM;
        }

    return P_ALARM;
    }
//-----------------------------------------------------------------------------
const char* tech_dev_error::get_group( tech_object::ERR_MSG_TYPES err_type ) 
    {
    switch ( err_type )
        {
        case tech_object::ERR_CANT_ON:
        case tech_object::ERR_ON_WITH_ERRORS:
            return "�����";

        case tech_object::ERR_OFF:
        case tech_object::ERR_OFF_AND_ON:
        case tech_object::ERR_DURING_WORK:
            return "���������";

        case tech_object::ERR_ALARM:
            return "�������";
        }

    return "?";
    }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
dev_errors_manager::dev_errors_manager(): errors_id( 0 )
    {
    }
//-----------------------------------------------------------------------------
int dev_errors_manager::save_as_Lua_str( char *str, u_int_2 &id )
    {
    str[ 0 ] = 0;

    bool is_new_error_state = false;

    for ( u_int i = 0; i < s_errors_vector.size(); i++ )
        {
        s_errors_vector[ i ]->save_as_Lua_str( str + strlen( str ),
            is_new_error_state );
        }

    if ( is_new_error_state )
        {
        errors_id++;        // ������ ����������.
        }

    id = errors_id; //����� �������� ���������� ��������� ������.

#ifdef DEBUG
#endif // DEBUG

    return 0;
    }
//-----------------------------------------------------------------------------
int dev_errors_manager::add_error( base_error  *s_error )
    {
    s_errors_vector.push_back( s_error );
    return 0;
    }
//-----------------------------------------------------------------------------
void dev_errors_manager::print()
    {
#ifdef DEBUG
    Print( "dev_errors_manager\n" );

    for ( u_int i = 0; i < s_errors_vector.size(); i++ )
        {
        s_errors_vector[ i ]->print();
        }

#endif // DEBUG
    }
//-----------------------------------------------------------------------------
void dev_errors_manager::reset_errors_params()
    {
    for ( u_int i = 0; i < s_errors_vector.size(); i++ )
        {
        s_errors_vector[ i ]->reset_errors_params();
        }
    }
//-----------------------------------------------------------------------------
void dev_errors_manager::set_cmd( unsigned int cmd, unsigned int object_type,
                                 unsigned int object_number,
                                 unsigned int object_alarm_number )
    {
    base_error *res = 0;

    // ����� ������� ����������.
    for ( u_int i = 0; i < s_errors_vector.size(); i++ )
        {
        if ( s_errors_vector[ i ]->get_object_type() == object_type &&
            s_errors_vector[ i ]->get_object_n() == object_number )
            {
            res = s_errors_vector[ i ];
            break;
            }
        }

    // ���������� �������.
    if ( res )
        {
        int result = res->set_cmd( cmd, object_alarm_number );
        if ( 0 == result )
        	{
            errors_id++; // C�������� ������ ����������.
        	}
        }
    else
        {
#ifdef DEBUG
        Print( "Error dev_errors_manager::set_cmd(...) - cmd = %u, object_type = %u,\
               object_number = %u, object_alarm_number = %u\n",
               cmd, object_type, object_number, object_alarm_number );
        Print( "Error object not found!\n" );
#endif // DEBUG
        }

#ifdef DEBUG
    //print();
#endif // DEBUG

    }
//-----------------------------------------------------------------------------
dev_errors_manager::~dev_errors_manager()
    {
    for ( u_int i = 0; i < s_errors_vector.size(); i++ )
        {
        delete s_errors_vector.at( i );
        s_errors_vector.at( i ) = 0;
        }
    }
//-----------------------------------------------------------------------------
dev_errors_manager* dev_errors_manager::get_instance()
    {
    static int is_init = 0;
    if ( 0 == is_init )
        {
        instance = new dev_errors_manager();
        is_init = 1;
        }

    return instance;
    }
//-----------------------------------------------------------------------------
#endif // PAC
