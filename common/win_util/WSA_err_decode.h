/// @file WSA_err_decode.h.h
/// @brief �������� ������� ����������� ������ ���������� ������ � �������� 
/// Windows.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#pragma once
/// @brief ����������� ��������� ������ ���������� WSA.
const char* WSA_Last_Err_Decode ();

/// @brief ����������� ������ ���������� WSA.
///
/// @param err_code [ in ] - ��� ������.
///
/// @return ������ � ������������ ������.
const char* WSA_Err_Decode ( int err_code );