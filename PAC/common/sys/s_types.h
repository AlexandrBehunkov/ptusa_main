/// @file s_types.h
/// @brief ��������� �������������� ����� ������� ����� ��� ����������� 
/// ������������� ��� ������������ ��� ����������� �� ����������� ����������, 
/// ����� ��� ����������� ����� ������� �������� ������.
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

#ifndef TYPES_H
#define TYPES_H

#if !defined WIN_OS && !defined LINUX_OS
#error You must define OS!
#endif 

#ifdef WIN_OS
#include "w_types.h"
#if _MSC_VER <= 1700            //Visual Studio 2012
#define snprintf _snprintf
#endif
#endif // WIN_OS

#ifdef LINUX_OS
#include "l_types.h"
#endif // LINUX_OS

//-----------------------------------------------------------------------------
typedef unsigned char     u_char;
typedef unsigned int      u_int;
typedef unsigned long int u_long;

extern int G_DEBUG;

#endif // TYPES_H
