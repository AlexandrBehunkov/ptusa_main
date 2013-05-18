/// @file l_console.h
/// @brief ������ � �������� � �� Linux. ����� ������ ���������� �������
/// ������ �� ��������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG    - ���������� c ������� ���������� ���������� � �������.@n@n
///
/// @par ������� ������:
/// @$Rev: 220 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-15 16:58:56#$.

#ifndef L_CONSOLE_H
#define L_CONSOLE_H

#include <time.h>
//-----------------------------------------------------------------------------
#ifdef DEBUG
#undef Print
#define Print    printf
#endif // DEBUG
//-----------------------------------------------------------------------------
extern time_t t_;
#define print_time t_ = time( 0 ); printf( "%02d.%02d.%02d %02d:%02d:%02d ",\
     localtime( &t_ )->tm_mday, localtime( &t_ )->tm_mon, localtime( &t_ )->tm_year,\
     localtime( &t_ )->tm_hour, localtime( &t_ )->tm_min, localtime( &t_ )->tm_sec ); printf
//-----------------------------------------------------------------------------
#endif // L_CONSOLE_H
