#ifndef iot_common_h__
#define iot_common_h__

/*!
 * \brief �������������� ��� �������, ������������ ������
 */
#define SOCKID_ALTIVAR 1000

/*!
 * \class i_iot_node
 *
 * \brief �����-��������� ��� ���������� ��������� � ��������� � ��������� ������������
 *
 */
class i_iot_node
{
public:
	i_iot_node();
	virtual ~i_iot_node();
};

/*!
 * \brief ����� ������� �������������
 */
void IOT_INIT();

/*!
 * \brief ����� ������� ����������� � ������� ����� ���������
 */
void IOT_EVALUATE();

/*!
 * \brief ����������� ��� ���������� ���������
 */
void IOT_FINAL();

#endif // iot_common_h__