/// @file dtime.h
/// @brief ������� ������ �� ��������.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c WIN_OS - ���������� ��� �� Windows.@n@n
/// 
/// @par ������� ������:
/// @$Rev: 223 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-17 09:39:32#$.

#ifndef DTIME_H
#define DTIME_H

#if !defined WIN_OS && !defined LINUX_OS
#error You must define OS!
#endif 

#include "s_types.h"

#include <time.h>

//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������� � ��������.
///
/// @return ������� �����.
/// @warning ����� ������������ � �������� � 01.01.1970, � 2038 ����������
/// ������������.
u_long get_sec();
//-----------------------------------------------------------------------------
/// @brief ��������� ������� � �������������.
///
/// ��� ��� �� 4 ��� ���������� ������������ � ������ ������������ � 0, �� ���
/// ���������� �������� ������� ������������ @ref get_delta_millisec.
///
/// @return ����� � ������� ������� ��������� � �������������.
u_long get_millisec();
//-----------------------------------------------------------------------------
/// @brief ��������� �������� ������� � �������������.
///
/// @param time1     - ��������� �����.
/// @return �������� ������� � �������������.
u_long get_delta_millisec( u_long time1 );
//-----------------------------------------------------------------------------
/// @brief �������� �������� �����.
///
/// @param ms - ����� ��������, ��.
void sleep_ms( u_int ms );
//-----------------------------------------------------------------------------
/// @brief ��������� ������� ���������� � �������.
///
/// @return ������� ���� � �����.
extern struct tm get_time();
//-----------------------------------------------------------------------------
struct stat_time
    {
    u_long all_time;
    u_long cycles_cnt;

    u_int  max_iteration_cycle_time;
    u_int  min_iteration_cycle_time;

    int print_cycle_last_h;

    stat_time() : all_time( 0 ), cycles_cnt( 1 ), max_iteration_cycle_time( 0 ),
	    min_iteration_cycle_time( 10000 )
	{
        time_t t_ = time( 0 );
        struct tm *timeInfo_ = localtime( &t_ );

        print_cycle_last_h = timeInfo_->tm_hour;
	}

    void clear()
	{
	all_time = 0;
	cycles_cnt = 1;
	max_iteration_cycle_time = 0;
	min_iteration_cycle_time = 10000;
	}
    };
#endif // DTIME_H
