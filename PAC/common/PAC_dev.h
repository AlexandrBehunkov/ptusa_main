//����� ��������.
//  �������� ������, ������� ������������ ������ � ����������� (PAC) ��� 
//����������� ������ � ������������.
//�������� DEFINE.
//  I7188   - ���������� ���������� ��� ����������� I7188.
//  I7188_E - ���������� ���������� ��� ����������� I7188E.
//  I7186_E - ���������� ���������� ��� ����������� I7186E.

#ifndef PAC_DEVICES_H
#define PAC_DEVICES_H

#include <string.h>
#include <stdio.h>
#include "g_device.h"

//-----------------------------------------------------------------------------
//����� ��������.
//  ������ ������, ������� ������������ ��� �������� ��������� ���������,
//  ������� �������������� �������� ���������� ����. ��������: ���������,
//  ��������� � �.�.
template < class data_type > class array_device: public i_simple_device
    {
    protected:
        u_int_4         sub_dev_cnt;    //���������� ������������.    
        char            *name;          //���.
        char            type;           //���.
        u_int_4         n;              //���������� �����.

        data_type*      prev_val;       //������ ���������� ��������.

    public:        
        array_device( u_int_4 n, char *new_name, u_int_2 new_subdev_cnt,
            char type );

        int  save_device( char *buff );
        int  save_state( char *buff );
        int  save_changed_state( char *buff );

        virtual data_type get_val( int idx ) = 0;

        void    print() const;
        u_int_4 get_n() const;

        int load_state( char *buff );
        int load_changed_state( char *buff );
        int load_device( char *buff );

        int     parse_cmd( char *buff  );
        u_int_4 get_idx();
        void    set_idx( u_int_4 new_idx );
    };
//-----------------------------------------------------------------------------
#endif // PAC_DEVICES_H
