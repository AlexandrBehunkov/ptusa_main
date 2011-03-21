/// @file console.h
/// @brief ������ � �������� ��� ����������� �� ���������� ����������. ����� 
/// ��������� ���������� ������� ������ �� ��������.
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

#ifndef CONSOLE_H
#define CONSOLE_H

#if !defined WIN_OS && !defined LINUX_OS && !defined MINIOS7
#error You must define OS!
#endif 

#include <string.h>
#include <stdio.h>

#include "s_types.h"
//-----------------------------------------------------------------------------
/// @brief ������ � �������.
#ifndef MINIOS7 
#define Print( ... )
#endif // MINIOS7

#ifdef WIN_OS
#include "w_console.h"
#endif // WIN_OS

#ifdef LINUX_OS
#include "l_console.h"
#endif // LINUX_OS

#ifdef MINIOS7
#include "mos7_console.h"
#endif // LINUX_OS
//-----------------------------------------------------------------------------
/// @brief �������� ������� �������.
///
/// @return 0 - ��� ������� ������.
/// @return 1 - ������ �������.
int  kb_hit();
//-----------------------------------------------------------------------------
/// @brief ���������� �������� ������� � �������.
///
/// @return - ������ ������� �������.
int  get_char();
//-----------------------------------------------------------------------------
/// @brief ����� ����� � ������� � �������� ����.
///
/// @param � - ��������� �����.
void print_binary( unsigned int c );
//-----------------------------------------------------------------------------
#endif // CONSOLE_H
