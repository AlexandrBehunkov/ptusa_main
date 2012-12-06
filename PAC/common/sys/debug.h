/// @file debug.h
/// @brief ��������� ����� ���������� �� �������� ������.
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

#ifndef DEBUG_H
#define DEBUG_H

///@brief ��������� ����� ���������� � ���������� ������. � ��������� ������
/// ������ �� ����������.
#define debug_break 
//-----------------------------------------------------------------------------
#ifdef DEBUG_IDE

#if defined LINUX_OS && !defined PAC_WAGO_750_860
#undef debug_break
#define debug_break asm ( "int" "3" ) 
#endif // defined LINUX_OS && !defined PAC_WAGO_750_860

#if defined WIN_OS
#undef debug_break
#define debug_break _asm { int 3 }
#endif // defined WIN_OS 

#endif // DEBUG
//-----------------------------------------------------------------------------
#endif // DEBUG_H
