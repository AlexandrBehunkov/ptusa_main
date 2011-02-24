/// @file types.h
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

#if !defined WIN_OS
#error You must define OS!
#endif // !defined WIN_OS

#ifdef WIN_OS
#include "w_types.h"
#endif // WIN_OS

typedef unsigned char     u_char;
typedef unsigned int      u_int;
typedef unsigned long int u_long;

#endif // TYPES_H