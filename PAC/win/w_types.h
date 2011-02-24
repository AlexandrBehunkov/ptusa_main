/// @file w_types.h
/// @brief ��������� �������������� ����� ������� ����� ��� ����������� 
/// ������������� ��� ������������ ��� ����������� �� ����������� ���������� 
/// ��� �� Windows.
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c WIN32 - ���������� ��� 32-� ��������� �� Windows.@n@n
/// 
/// @par ������� ������:
/// @$Rev: 223 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-02-17 09:39:32#$.

#ifndef W_TYPES_H
#define W_TYPES_H

#if !defined WIN32
#error You should define the type of OS Windows!
#endif // !defined WIN32

#ifdef WIN32
typedef short int           int_2;
typedef int                 int_4;
typedef unsigned short int  u_int_2;
typedef unsigned int        u_int_4;
#endif // WIN32

#endif // W_TYPES_H
