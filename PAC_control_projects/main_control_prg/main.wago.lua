require "sys_wago" --��������� �������.
-- ----------------------------------------------------------------------------
editor_version = 12
file_version   = 74

--���� WAGO
nodes = 
    {
        {
        ntype   = 1,
        address = 1,
        modules = 
            {
            { 504, '', '', '', '', '' }, 
            { 530, '', '', '', '', '' }, 
            { 402, '', '', '', '', '' }, 
            { 430, '', '', '', '', '' }, 
            { 530, '', '', '', '', '' }, 
            { 504, '', '', '', '', '' }, 
            { 554, '', '', '', '', '' }, 
            { 554, '', '', '', '', '' }, 
            { 638, '', '', '', '', '' }, 
            { 466, '', '', '', '', '' }, 
            { 461, '', '', '', '', '' }, 
            { 461, '', '', '', '', '' }, 
            { 600, '', '', '', '', '' }, 
            }
        },
    }

--����������
devices = 
    {
        {
        descr   = 'VC100 ���. 1-�� ������ ���������',
        dtype   = 8,
        subtype = 0,
        number  = 100,
        AO  = 
            {
                {
                node   = 0,
                offset = 0
                }
            },
        },

        {
        descr   = 'VC101 ���. 2-�� ������ ���������',
        dtype   = 8,
        subtype = 0,
        number  = 101,
        AO  = 
            {
                {
                node   = 0,
                offset = 1
                }
            },
        },

        {
        descr   = '������� ������ N101',
        dtype   = 8,
        subtype = 0,
        number  = 1001,
        AO  = 
            {
                {
                node   = 0,
                offset = 2
                }
            },
        },

        {
        descr   = '������� ��������',
        dtype   = 7,
        subtype = 0,
        number  = 101,
        AI  = 
            {
                {
                node   = 0,
                offset = 0
                }
            },
        },

        {
        descr   = '������ ��������',
        dtype   = 5,
        subtype = 0,
        number  = 101,
        AI  = 
            {
                {
                node   = 0,
                offset = 2
                }
            },
        par = { 0, 30 } 
        },

        {
        descr   = '�.�. ����� � ���������',
        dtype   = 3,
        subtype = 0,
        number  = 1,
        DI  = 
            {
                {
                node   = 0,
                offset = 0
                }
            },
        },

        {
        descr   = '��.�. ����� � ���������',
        dtype   = 3,
        subtype = 0,
        number  = 2,
        DI  = 
            {
                {
                node   = 0,
                offset = 2
                }
            },
        },

        {
        descr   = '��.�. ����� � ���������',
        dtype   = 3,
        subtype = 0,
        number  = 3,
        DI  = 
            {
                {
                node   = 0,
                offset = 1
                }
            },
        },

        {
        descr   = '�1 - �������������',
        dtype   = 2,
        subtype = 0,
        number  = 1,
        DO  = 
            {
                {
                node   = 0,
                offset = 18
                }
            },
        DI  = 
            {
                {
                node   = 0,
                offset = 7
                }
            },
        },

        {
        descr   = 'N100 - ����� ������� ������� ���������',
        dtype   = 1,
        subtype = 0,
        number  = 100,
        DO  = 
            {
                {
                node   = 0,
                offset = 2
                }
            },
        DI  = 
            {
                {
                node   = 0,
                offset = 4
                }
            },
        },

        {
        descr   = 'N101 - ����� ��������',
        dtype   = 1,
        subtype = 0,
        number  = 101,
        DO  = 
            {
                {
                node   = 0,
                offset = 1
                }
            },
        DI  = 
            {
                {
                node   = 0,
                offset = 6
                }
            },
        },

        {
        descr   = '�.1-12 ������ � ������',
        dtype   = 10,
        subtype = 0,
        number  = 21,
        DI  = 
            {
                {
                node   = 0,
                offset = 8
                }
            },
        },

        {
        descr   = '�.32-38 ������ � �������',
        dtype   = 10,
        subtype = 0,
        number  = 22,
        DI  = 
            {
                {
                node   = 0,
                offset = 10
                }
            },
        },

        {
        descr   = '�� �.1-12 ������� ����',
        dtype   = 10,
        subtype = 0,
        number  = 23,
        DI  = 
            {
                {
                node   = 0,
                offset = 5
                }
            },
        },

        {
        descr   = '������ ����� R3 �� �.32-38',
        dtype   = 10,
        subtype = 0,
        number  = 24,
        DI  = 
            {
                {
                node   = 0,
                offset = 3
                }
            },
        },

        {
        descr   = '������� ������������� �� �����',
        dtype   = 10,
        subtype = 0,
        number  = 25,
        DI  = 
            {
                {
                node   = 0,
                offset = 9
                }
            },
        },

        {
        descr   = '������� ������� ����� �. ���. �7',
        dtype   = 10,
        subtype = 0,
        number  = 26,
        DI  = 
            {
                {
                node   = 0,
                offset = 11
                }
            },
        },

        {
        descr   = '����� �������� � 1-��� ������� ���������',
        dtype   = 4,
        subtype = 0,
        number  = 100,
        AI  = 
            {
                {
                node   = 0,
                offset = 4
                }
            },
        },

        {
        descr   = '����� �������� �� 2-��� ������� ���������',
        dtype   = 4,
        subtype = 0,
        number  = 101,
        AI  = 
            {
                {
                node   = 0,
                offset = 5
                }
            },
        },

        {
        descr   = '������������� 1-��� ������� ���������',
        dtype   = 4,
        subtype = 0,
        number  = 102,
        AI  = 
            {
                {
                node   = 0,
                offset = 6
                }
            },
        },

        {
        descr   = '������',
        dtype   = 11,
        subtype = 0,
        number  = 1,
        DO  = 
            {
                {
                node   = 0,
                offset = 11
                }
            },
        },

        {
        descr   = '���. �2 ������ � �������',
        dtype   = 11,
        subtype = 0,
        number  = 21,
        DO  = 
            {
                {
                node   = 0,
                offset = 4
                }
            },
        },

        {
        descr   = '������ �� ��������� N2',
        dtype   = 11,
        subtype = 0,
        number  = 22,
        DO  = 
            {
                {
                node   = 0,
                offset = 6
                }
            },
        },

        {
        descr   = '�.�. � �. 1-12',
        dtype   = 11,
        subtype = 0,
        number  = 23,
        DO  = 
            {
                {
                node   = 0,
                offset = 8
                }
            },
        },

        {
        descr   = '����� ���� ��� �.32-38',
        dtype   = 11,
        subtype = 0,
        number  = 24,
        DO  = 
            {
                {
                node   = 0,
                offset = 3
                }
            },
        },

        {
        descr   = '�������� �������� ������������� �� �����',
        dtype   = 11,
        subtype = 0,
        number  = 25,
        DO  = 
            {
                {
                node   = 0,
                offset = 14
                }
            },
        },

        {
        descr   = '������� ����� �. ���. �7 ������',
        dtype   = 11,
        subtype = 0,
        number  = 26,
        DO  = 
            {
                {
                node   = 0,
                offset = 12
                }
            },
        },

        {
        descr   = '���� � �.32-38',
        dtype   = 11,
        subtype = 0,
        number  = 100,
        DO  = 
            {
                {
                node   = 0,
                offset = 13
                }
            },
        },

        {
        descr   = '����� ���� � �.32-38',
        dtype   = 11,
        subtype = 0,
        number  = 101,
        DO  = 
            {
                {
                node   = 0,
                offset = 15
                }
            },
        },

        {
        descr   = '������� � �.32-38',
        dtype   = 11,
        subtype = 0,
        number  = 102,
        DO  = 
            {
                {
                node   = 0,
                offset = 17
                }
            },
        },

        {
        descr   = '���������� � �.32-38',
        dtype   = 11,
        subtype = 0,
        number  = 103,
        DO  = 
            {
                {
                node   = 0,
                offset = 19
                }
            },
        },

        {
        descr   = 'V22 - ������ ������ �������� 1-��� �����',
        dtype   = 0,
        subtype = 1,
        number  = 22,
        DO  = 
            {
                {
                node   = 0,
                offset = 20
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'V23 - ������ ������ �������� 2-��� �����',
        dtype   = 0,
        subtype = 1,
        number  = 23,
        DO  = 
            {
                {
                node   = 0,
                offset = 22
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'V94 - ������ (NO)',
        dtype   = 0,
        subtype = 1,
        number  = 94,
        DO  = 
            {
                {
                node   = 0,
                offset = 0
                }
            },
        par = { 1 } 
        },

        {
        descr   = 'V95 - � �����',
        dtype   = 0,
        subtype = 1,
        number  = 95,
        DO  = 
            {
                {
                node   = 0,
                offset = 5
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'V96+V97 - ����',
        dtype   = 0,
        subtype = 1,
        number  = 96,
        DO  = 
            {
                {
                node   = 0,
                offset = 16
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'V97 - ����',
        dtype   = 0,
        subtype = 1,
        number  = 97,
        DO  = 
            {
                {
                node   = 0,
                offset = 16
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'V98(99) �������� � �. ���. ������ �� ���.�2 (NO)',
        dtype   = 0,
        subtype = 1,
        number  = 98,
        DO  = 
            {
                {
                node   = 0,
                offset = 10
                }
            },
        par = { 1 } 
        },

        {
        descr   = '���. ������ � ����� ���.�2',
        dtype   = 0,
        subtype = 1,
        number  = 99,
        DO  = 
            {
                {
                node   = 0,
                offset = 10
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'R3V7(8) R3- � ���. �2 (NC)',
        dtype   = 0,
        subtype = 1,
        number  = 54307,
        DO  = 
            {
                {
                node   = 0,
                offset = 7
                }
            },
        par = { 1 } 
        },

        {
        descr   = 'R3V8 R3- � ���. �2(N0)',
        dtype   = 0,
        subtype = 1,
        number  = 54308,
        DO  = 
            {
                {
                node   = 0,
                offset = 7
                }
            },
        par = { 0 } 
        },

        {
        descr   = 'R3V9(10) R3+ �� ���. �2 (NC)',
        dtype   = 0,
        subtype = 1,
        number  = 54309,
        DO  = 
            {
                {
                node   = 0,
                offset = 9
                }
            },
        par = { 1 } 
        },

        {
        descr   = 'R3V10 R3+ �� ���. �2(N0)',
        dtype   = 0,
        subtype = 1,
        number  = 54310,
        DO  = 
            {
                {
                node   = 0,
                offset = 9
                }
            },
        par = { 0 } 
        },

    }

init_tech_objects_modes = function()
	return
	{
		{
		n = 1,
		name = 'Grebenka',
		modes = 
			{
			[ 1 ] = 
				{
				name = '����� �. ���. �7',
				opened_devices = 
					{
					V = { 54309, 54310 }
					},
				pair_dev = 
					{
					pair = { { 26, 26 } }
					},
				},
			},
		},
		{
		n = 2,
		name = '���������',
		modes = 
			{
			[ 1 ] = 
				{
				name = '������ ���������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '������ ���������',
						},
					},
				},
			[ 2 ] = 
				{
				name = '����� ���������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '����� ���������',
						},
					},
				},
			[ 3 ] = 
				{
				name = '����� �� �����',

				steps = 
					{
					[ 1 ] = 
						{
						name = '����� �� �����',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310, 95 }
							},
						},
					[ 2 ] = 
						{
						name = '����� �� ����� �������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310, 95 }
							},
						},
					[ 3 ] = 
						{
						name = '����� � ������������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310, 95 }
							},
						},
					},
				},
			[ 4 ] = 
				{
				name = '���������� ���������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '���������� ���������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 98, 99 }
							},
						},
					},
				},
			[ 5 ] = 
				{
				name = '������������� �������� � ���������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '������������� �������� � ���������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310, 95 }
							},
						},
					[ 2 ] = 
						{
						name = '����������  �������� �� ��������������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 95, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310 }
							},
						},
					[ 3 ] = 
						{
						name = '���������� ���������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 95, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310 }
							},
						},
					[ 4 ] = 
						{
						name = '������������� ����� ����',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 95, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310 }
							},
						},
					},
				},
			[ 6 ] = 
				{
				name = '������������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '������������',
						opened_devices = 
							{
							V = { 54307, 54308, 94, 95, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54309, 54310 }
							},
						},
					},
				},
			[ 7 ] = 
				{
				name = '����������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '����������',
						opened_devices = 
							{
							V = { 94, 95, 98, 99 }
							},
						closed_devices = 
							{
							V = { 54307, 54308, 54309, 54310 }
							},
						},
					},
				},
			[ 8 ] = 
				{
				name = '���������� ��������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '���������� ��������',
						opened_devices = 
							{
							V = { 54307, 54308 }
							},
						},
					[ 2 ] = 
						{
						name = '���������� ���������',
						opened_devices = 
							{
							V = { 54307, 54308 }
							},
						},
					[ 3 ] = 
						{
						name = '���������� ����� ����',
						opened_devices = 
							{
							V = { 54307, 54308 }
							},
						},
					[ 4 ] = 
						{
						name = '���������� ����� �����  �� ���������',
						opened_devices = 
							{
							V = { 54307, 54308 }
							},
						},
					},
				},
			[ 9 ] = 
				{
				name = '�����������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '�����������',
						closed_devices = 
							{
							V = { 54307, 54308, 54309, 54310 }
							},
						},
					},
				},
			[ 10 ] = 
				{
				name = '�������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '�������',
						},
					},
				},
			[ 11 ] = 
				{
				name = '������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '������',
						},
					},
				},
			[ 12 ] = 
				{
				name = '�������������',

				steps = 
					{
					[ 1 ] = 
						{
						name = '�������������',
						opened_devices = 
							{
							V = { 54307, 54308 }
							},
						},
					[ 2 ] = 
						{
						name = '�������������',
						opened_devices = 
							{
							V = { 54307, 54308 }
							},
						},
					},
				},
			[ 13 ] = 
				{
				name = '������������� �� ��������� N7',

				steps = 
					{
					[ 1 ] = 
						{
						name = '������������� �� ��������� N7',
						opened_devices = 
							{
							V = { 54307, 54308, 54309, 54310 }
							},
						},
					},
				},
			}
		}
    }
end
