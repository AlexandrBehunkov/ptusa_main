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

#ifndef SIMPLE_LOG
#include <syslog.h>
#endif

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
#ifdef SIMPLE_LOG
        printf("%s\n\r",msg);
#else
        //Using openlog() because kbus functions also use it
        //(openlog( "kbus", ...)).
        openlog( "ptusa", LOG_PID | LOG_PERROR, LOG_USER );

        syslog( (int) priority, "%s", msg );
        closelog();
#endif
        }

    l_log()
        {
        }
};

#endif // L_LOG_H
