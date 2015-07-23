/// @file l_log.h
/// @brief ������ � ����������������� �������, ������ � �.�. ��� �� Linux.
///
/// @author  ������ ������� ���������.
///
/// @par ������� ������:
/// @$Rev: 332 $.\n
/// @$Author: id $.\n
/// @$Date:: 2011-08-24 09:59:40#$.

#ifndef L_LOG_H
#define L_LOG_H

#include "log.h"

#include <syslog.h>

#include <stdio.h>
//-----------------------------------------------------------------------------
/// @brief ������ � ��������.
///
/// ������� ������������ ����� ��� �������.
/// �������� �������� ������ ������ - ����� � ���, �.�.
///
class l_log: public i_log
    {
    public:

    virtual ~l_log()
        {
        }

    void virtual write_log(PRIORITIES priority)
        {
        //Using openlog() because kbus functions also use it
        //(openlog( "kbus", ...)).
        openlog( "ptusa", LOG_PID | LOG_PERROR, LOG_USER );

        syslog( (int) priority, "%s", msg );
        closelog();
        }

    l_log()
        {
        }
};

#endif // L_LOG_H
