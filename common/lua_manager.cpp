#include "lua_manager.h"
//-----------------------------------------------------------------------------
auto_smart_ptr< lua_manager > lua_manager::instance;
//-----------------------------------------------------------------------------
lua_manager* lua_manager::get_instance()
    {
    static int is_init = 0;
     if ( 0 == is_init )
        {
        instance = new lua_manager();
        is_init = 1;
        }

     return instance;
    }
//-----------------------------------------------------------------------------
int lua_manager::init()
    {
    //-������������� Lua.
    L = lua_open();   // Create Lua context.

    if ( NULL == L )
        {
        printf( "Error creating Lua context.\n" );
        return 1;
        }

    luaL_openlibs( L );    // Open standard libraries.
    tolua_PAC_dev_open( L );

#if defined WIN_OS && defined DEBUG
    if( luaL_loadfile( L, "../main.lua" ) != 0 )
#else
    if( luaL_loadfile( L, "main.lua" ) != 0 )
#endif // defined OS_WIN && defined DEBUG

        {
#ifdef DEBUG
        Print( "Init lua error!\n" );
#endif // DEBUG
        return 1;
        }
    //-������������� Lua.--!>

    int i_line = lua_pcall( L, 0, LUA_MULTRET, 0 );
    if ( i_line != 0 )
        {
        return 1;
        }

    return 0;
    }
//-----------------------------------------------------------------------------
lua_manager::~lua_manager()
    {
     if ( L )
        {
        lua_close( L );
        L = NULL;
        }
    }
//-----------------------------------------------------------------------------
int lua_manager::void_exec_lua_method( const char *object_name,
    const char *function_name, const char *c_function_name ) const
    {
    int res = 0;
    if ( 0 == exec_lua_method( object_name, function_name, 0, 0, 0 ) )
        {
        }
    else
        {
        res = 1;
#ifdef DEBUG
        Print( "Error during C++ call - \"%s\"\n", c_function_name );
#endif // DEBUG
        }

    return res;
    }
//-----------------------------------------------------------------------------
int lua_manager::int_exec_lua_method( const char *object_name,
    const char *function_name, int param, const char *c_function_name ) const
    {
    int res = 0;
    if ( 0 == exec_lua_method( object_name, function_name, param ) )
        {
        res = tolua_tonumber( L, -1, 0 );
        lua_remove( L, -1 );
        }
    else
        {
#ifdef DEBUG
        Print( "Error during C++ call - \"%s\"\n", c_function_name );
#endif // DEBUG
        }

    return res;
    }
//-----------------------------------------------------------------------------
void* lua_manager::user_object_exec_lua_method( const char *object_name,
    const char *function_name, int param, const char *c_function_name ) const
    {
    void* res = NULL;
    if ( 0 == exec_lua_method( object_name, function_name, param ) )
        {
        res = tolua_tousertype( L, -1, NULL );
        lua_remove( L, -1 );
        }
    else
        {
#ifdef DEBUG
        Print( "Error during C++ call - \"%s\"\n", c_function_name );
#endif // DEBUG
        }

    return res;
    }
//-----------------------------------------------------------------------------
int lua_manager::exec_lua_method( const char *object_name,
    const char *function_name, int param, int is_use_param,
    int is_use_lua_return_value ) const
    {
//    static int is_init = 0;
//    if ( 0 == is_init )
//        {
//        lua_pushcclosure( L, error_trace, 0 );
//        instance->err_func = lua_gettop( L );
//        is_init = 1;
//        }

    lua_pushcclosure( instance->L, error_trace, 0 );
    instance->err_func = lua_gettop( L );

    lua_getfield( L, LUA_GLOBALSINDEX, object_name );
    lua_getfield( L, -1, function_name );
    lua_remove( L, -2 );
    lua_getfield( L, LUA_GLOBALSINDEX, object_name );
    int param_count = 1;
    if ( is_use_param )
        {
        lua_pushnumber( L, param );
        param_count++;
        }
    int results_count = is_use_lua_return_value == 1 ? 1 : 0;
    int res = lua_pcall( L, param_count, results_count, err_func );

    lua_remove( L, -2 );

    return res;
    }
//-----------------------------------------------------------------------------
int lua_manager::error_trace( lua_State * L )
    {
#ifdef DEBUG
    Print( "\t%s\n", lua_tostring( L, -1 ) );
    lua_remove( L, -1 );

    Print( "\tstack traceback:\n" );
    lua_Debug ar;
    int level = 1;

    while( lua_getstack( L, level, &ar ) )
        {
        lua_getinfo( L, "Sln", &ar );
        Print( "\t\t%s:%d: in function '%s'\n",
            ar.source, ar.linedefined, ar.name );
        level++;
        }
#endif // DEBUG

    return 0;
    }
//-----------------------------------------------------------------------------
