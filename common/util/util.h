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

#include <stddef.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
#if defined LINUX_OS && !defined PAC_WAGO_750_860
#define debug_break asm ( "int" "3" ) ///< ��������� ����� ����������.
#endif // defined LINUX_OS

#ifdef WIN_OS
#define debug_break _asm { int 3 }
#endif
//-----------------------------------------------------------------------------
size_t strlcpy( char *dst, const char *src, size_t siz );
//-----------------------------------------------------------------------------
/// @brief ���������� ������ � ����������.
///
/// ����������� �������� �� ������ � null ����������.
template < class type > class smart_ptr
    {
    protected:
        type* pointer;

    public:
        /// @brief ����������� �� ���������.
        smart_ptr() : pointer( NULL )
            {
            }

        /// @brief ����������� �� ������ ���������.
        smart_ptr( type* p ) : pointer( p )
            {
            }

        /// @brief ����������� �����������.
        smart_ptr( const smart_ptr< type > &ptr ) : pointer( ptr.pointer )
            {
            }

        /// @brief �������� ������������ ���������.
        smart_ptr< type >& operator = ( type* new_t )
            {
            pointer = new_t;

            return *this;
            }

        /// @brief �������� ���������� ���� � ���������.
        operator type*()
            {
            if ( pointer == NULL )
                {
    #ifdef DEBUG

    #ifdef PAC_PC
                debug_break;
    #endif // PAC_PC

                printf( "smart_ptr - pointer is NULL!\n" );
    #endif // DEBUG
                }

            return pointer;
            }

        /// @brief �������� ��������� � ������ ������.
        type* operator->()
            {
            if ( pointer == NULL )
                {
    #ifdef DEBUG
                printf( "smart_ptr - dereferencing NULL!\n" );

    #ifdef PAC_PC
                debug_break;
    #endif // PAC_PC

    #endif // DEBUG
                }
            return pointer;
            }

    private:
        /// @brief �������� �������� �������������� - ������ �������������.
        type* operator[] ( unsigned int index )
            {
            return pointer;
            }

        /// @brief �������� �������� ������������ - ������ �������������.
        smart_ptr< type >& operator = ( const smart_ptr< type > &new_t )
            {
            return *this;
            }
    };
//-----------------------------------------------------------------------------
/// @brief ���������� ������ � ����������.
///
/// �������������� ������������ ������ ��� �����������, ���������������� �������.
template < class type > class auto_smart_ptr : public smart_ptr< type >
    {
    public:
        /// @brief ����������� �� ���������.
        auto_smart_ptr() : smart_ptr< type >( NULL )
            {
            }

        /// @brief ����������� �� ������ ���������.
        auto_smart_ptr( type* p ) : smart_ptr< type >( p )
            {
            }

        void free()
            {
            if ( smart_ptr< type >::pointer )
                {
                delete smart_ptr< type >::pointer;
                smart_ptr< type >::pointer = NULL;
                }
            }

        virtual ~auto_smart_ptr()
            {
            free();
            }

        /// @brief �������� ������������ ���������.
        auto_smart_ptr< type >& operator = ( type* new_t )
            {
            if ( smart_ptr< type >::pointer )
                {
                delete smart_ptr< type >::pointer;
                }
            smart_ptr< type >::pointer = new_t;
            return *this;
            }
    };
//-----------------------------------------------------------------------------
