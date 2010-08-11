/// @file util.h
/// @brief ����������� ��������� �������� �������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev: 96 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-07-09 15:57:50#$.

//-----------------------------------------------------------------------------
#if defined LINUX || defined UCLINUX
#define debug_break asm ( "int" "3" ) ///< ��������� ����� ����������.
#endif // defined LINUX || defined UCLINUX
//-----------------------------------------------------------------------------
/// @brief ���������� ������ � ����������.
///
/// ����������� �������� �� ������ � null ����������.
template < class type > class smart_ptr
    {
    protected:
        type* pointer;

    public:
        smart_ptr() : pointer( NULL )
            {
            }

        smart_ptr( type* p ) : pointer( p )
            {
            }

        smart_ptr( const smart_ptr< type > &ptr ) : pointer( ptr.pointer )
            {
            }

        smart_ptr< type >& operator = ( type* new_t )
            {
            pointer = new_t;

            return *this;
            }

        operator type*()
            {
            if ( pointer == NULL )
                {
    #ifdef DEBUG

    #ifdef DEBUG_TRACE
                debug_break;
    #endif // DEBUG_TRACE

                printf( "smart_ptr - pointer is NULL!\n" );
    #endif // DEBUG
                }

            return pointer;
            }

        type* operator->()
            {
            if ( pointer == NULL )
                {
    #ifdef DEBUG
                printf( "smart_ptr - dereferencing NULL!\n" );

    #ifdef DEBUG_TRACE
                debug_break;
    #endif // DEBUG_TRACE

    #endif // DEBUG
                }
            return pointer;
            }

    private:
        type* operator[] ( unsigned int index )
            {
            return pointer;
            }

        smart_ptr< type >& operator = ( const smart_ptr< type > &new_t )
            {
            return *this;
            }

    };
//-----------------------------------------------------------------------------
/// @brief ���������� ������ � ����������.
///
/// ���������������� ������ ��� ����������� �������.
template < class type > class auto_smart_ptr : public smart_ptr< type >
    {
    public:
        auto_smart_ptr() : smart_ptr< type >( NULL )
            {
            }

        auto_smart_ptr( type* p ) : smart_ptr< type >( p )
            {
            }

        virtual ~auto_smart_ptr()
            {
            if ( smart_ptr< type >::pointer ) delete smart_ptr< type >::pointer;
            smart_ptr< type >::pointer = NULL;
            }

        auto_smart_ptr< type >& operator = ( type* new_t )
            {
            smart_ptr< type >::pointer = new_t;
            return *this;
            }
    };
//-----------------------------------------------------------------------------
