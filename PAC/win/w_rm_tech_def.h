/// @file w_rm_tech_def.h
/// @brief ����������� ���������������� ��������������� �������� ��� ����������
/// �������� (����� ������-����������) ��� Windows.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG   - ���������� c ������� ���������� ���������� � �������.
///
/// @par ������� ������:
/// @$Rev: 678 $.\n
/// @$Author: id $.\n
/// @$Date:: 2013-05-29 16:44:39#$.

#ifndef W_RM_TECH_DEFINITION_H
#define W_RM_TECH_DEFINITION_H

#include <winsock2.h>

#include "dtime.h"
#include "rm_tech_def.h"
#include "WSA_err_decode.h"

#include "lua_manager.h"

//-----------------------------------------------------------------------------
class win_rm_cmmctr: public i_rm_cmmctr
    {
    public:
        /// <summary>
        /// �����������.
        /// </summary>
        /// <param name="name">��� ���������� PAC.</param>
        /// <param name="IP_address">IP ����� ���������� PAC.</param>
        /// <returns></returns>
        win_rm_cmmctr( char* name, char* IP_address, int remote_PAC_id );

        /// @brief ������� �������� ������� PAC.
        int send_2_PAC( int service_n, const char *cmd_str, int length );

        /// @brief ����� ������� �� PAC.
        int evaluate();

    private:
        /// <summary>
        /// ������������� ������� ����������.
        /// </summary>
        /// <returns>0 - �����.</returns>
        /// <returns>1 - ������.</returns>
        int net_init();

        /// <summary>
        /// ����������� � ���������� PAC.
        /// </summary>
        /// <returns>0 - �����.</returns>
        /// <returns>1 - ������.</returns>
        int connect_to_PAC();

        /// <summary>
        /// ���������� �� ���������� PAC.
        /// </summary>
        void disconnect();

        // @brief ���� �������� ������������� ����.
        bool is_initialized;

        // @brief ���� ��������� ���������� � ��������� PAC.
        bool is_connected;

        // @brief ����� ���������� �������� ������ � ��������� PAC.
        u_long last_transfer_time;

        // @brief ���� ������ ����� � ��������� PAC.
        bool cmctr_err;   

        // @brief ����� ����� � ��������� PAC.
        int remote_PAC_socket;

        int port;

        UCHAR pidx;

        // @brief ���� ��������� ��������� ���������� PAC.
        bool got_devices;

        /// @brief ����� ������� ��������� � PAC. ��������������� � 0 ���
        /// �������� PAC.
        u_int_2 devices_request_id; 
    };
//-----------------------------------------------------------------------------
#endif // W_RM_TECH_DEFINITION_H
