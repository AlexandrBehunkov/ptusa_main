/// @file smart_ptr.h
/// @brief ����������� ��������� �������� �������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef _UTIL_H
#define _UTIL_H

#include "console.h"
#include "types.h"
#include "debug.h"

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
                Print( "smart_ptr - pointer is NULL!\n" );
                debug_break;
                }

            return pointer;
            }

        /// @brief �������� ��������� � ������ ������.
        type* operator->()
            {
            if ( pointer == NULL )
                {
                Print( "smart_ptr - dereferencing NULL!\n" );
                debug_break;
                }
            return pointer;
            }

        /// @brief �������� ��������� � ������ ������.
        type* operator->() const
            {
            if ( pointer == NULL )
                {
                Print( "smart_ptr - dereferencing NULL!\n" );
                debug_break;
                }
            return pointer;
            }

        inline bool is_null()
            {
            return pointer == NULL;
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
#endif // _UTIL_H
