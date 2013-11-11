#ifndef CIP_TECH_DEF
#define CIP_TECH_DEF

#include "tech_def.h"
#include "g_device.h"
#include "PID.h"


enum workParameters 
	{
	P_CONC_RATE = 1,	//������ ����������� ��� ���������
	P_ZAD_PODOGR,	//������� ������� ���������
	P_ZAD_FLOW,		//������� ������� �������
	P_VRAB,			//�������� ����� ��������
	P_MAX_OPER_TM,	//������������ ����� ������� ��������
	P_OP_TIME_LEFT,	//������ ������� ������� ��������
	P_CONC,			//������� ������������
	P_SUM_OP,		//������ � ������ ��������
	P_ZAD_CONC,		//�������� ������������
	P_LOADED_RECIPE,	//����������� ������

	P_PROGRAM,		//��������� �����

	P_CUR_REC,		//������� ��������� ������
	P_RET_STATE,	//��������� ����������� ������
	P_SELECT_REC,	//�������� �������
	P_SELECT_PRG,	//�������� ���������

	PV1,			//����� V1 (�� ������ �� �������)
	PV2,			//����� V2 (�� ������� �� ������)
	P_OBJ_TYPE,		//��� ������� (����, ����� � ��.)
	P_FLOW,			//������
	P_PODP_CIRC,	//����������� �� ����� �� ����� ����������  
	P_DELTA_TR,		//������� ���������� �� ������ � �������� (���� ������ ��������, �� ���������� ������ ������� ����������)

	P_T_WP,	//����������� ���������������� �������������
	P_T_WSP,	//����������� ������������� �������� ����� ������
	P_T_WKP,	//����������� ������������� �������� ����� �������
	P_T_WOP,	//����������� �������������� �������������
	P_T_S,	//����������� ����� �������
	P_T_K,	//����������� ����� ��������
	P_T_D,	//����������� ����������� �� ����

	P_T_SANITIZER,	//����������� ����������� ��������������� ���������
	P_DOP_V_PR_OP,	//�������������� ����� ���������������� �������������
	P_DOP_V_AFTER_S,	//�������������� ����� ������������� ����� ������
	P_DOP_V_AFTER_K,	//�������������� ����� ������������� ����� �������
	P_DOP_V_OK_OP,		//�������������� ����� �������������� �������������

	P_RET_STOP,	//����� �� ����� V1 ��� ��������� ����������� ������
	P_V_RAB_ML,   //���������� ������ ���� �������� ������ ��� ������� ������������� ���������� ����� ����������
	PV_RET_DEL,	//����� ������� ������ ����� ������ ������ V2 �������� ���������� �����
	PTM_OP,	//����� ���������� ��� ������������� (�������� �� ������������)
	PTM_S,	//����� ���������� �� ������
	PTM_K,	//����� ���������� �� �������
	PTM_S_SK,	//����� ���������� �� ������ ��� ����� �������+������
	PTM_K_SK,	//����� ���������� �� ������� ��� ����� �������+������
	PTM_D,	//����� ����������� �� ����
	PTM_SANITIZER, //����� ����������� ��������������� ���������
	PTM_SANITIZER_INJECT, //����� �������� ���������������� �������� ��� �����������

	P_N_RET,		//����� ����������� ������
	P_N_UPR,		//����� �������� �����
	P_OS,			//�������� �����
	P_OBJ_EMPTY,	//������ "������ ���������"
	P_PROGRAM_MASK, //����� ������� �����
	P_T_CLEAN_RINSING, //����������� ������������� ������ �����
	P_V_CLEAN_RINSING, //����� ������������� ������ �����
	P_RESERV_START,


	STP_QAVS,		//������� ������������ ������
	STP_QAVK,		//������� ������������ �������
	STP_WC,			//������ ����
	STP_WS,			//��������� ����
	STP_LV,			//������������� ��������
	STP_WC_INST_WS,	//������ ����, �������������� �� ������� �������� ���������
	STP_WASH_START, //����� ������ ����� (������������ �������� ��� ��������)
	STP_STEPS_OVER,	//���������� ����������� ��������
	STP_RESETSTEP,	//���, �� ������� ��������
	STP_ERRCOUNT,	//���������� ������
	};

	enum storedParameters
		{
		P_TM_RET_IS_EMPTY = 1, //����� ���������� ������� �� �������� ��� ��������� ����������� �������
		P_V_LL_BOT,	//���������� ������ �� ���������� ������� ������ � ����� �� ���������� �������� "����������� �����"
		P_R_NO_FLOW,	//����������� ������ ��� ������� �������, ��� ��� ���
		P_TM_R_NO_FLOW,	//����� ������������� ������ "��� ������� �� ������"
		P_TM_NO_FLOW_R,	//����� ��������� ������ "��� ������� �� ��������"	
		P_TM_NO_CONC,	//����� ��������� ������ "��� ������������ � ���������� �����"
		};


class cipline_tech_object: public tech_object
{
protected:
	int ncmd;

	int ret_overrride; //���� ��������������� ���������/���������� ����������� �������
	int concentration_ok; //���� ������������ �� ��������
	int return_ok; //���� ������ �� ��������
	int tank_is_empty; //������������ ��� ����, ����� ����������, ����� �� ��������� ���������� �����
	unsigned long sort_delay; //������ �������� ��� ���������� ��������� (����� �� ������������� ����������)
	unsigned long steam_valve_delay; //������ �������� ��������� ��������� ������� ���� (����� �� ���������/���������� �� ��������� ��������� �����������)
	unsigned long bachok_lvl_err_delay; //�������� ��� ��������� ������ ������ �����


	unsigned long enddelayTimer;
	int valvesAreInConflict;
	virtual void resetProgramName();
	virtual void resetRecipeName();
	virtual void resetProgramList(unsigned long programmask = 0xB00);
	virtual void formProgramList(unsigned long programmask);
	virtual void loadProgramFromList(int selectedPrg);
	virtual void closeLineValves();
	int isTank();
	int isLine();
	int getValvesConflict();

	//���������� �������� �������
	int pump_control;
	//TPumpController* NPC;
    public:
    cipline_tech_object( const char* name, u_int number, u_int type, const char* name_Lua,
            u_int states_count,
            u_int timers_count,
            u_int par_float_count, u_int runtime_par_float_count,
            u_int par_uint_count, u_int runtime_par_uint_count );
    i_DO_device* V1;

	static saved_params<float, true>* parpar;

	PID* PIDF;
	PID* PIDP;
	void initline();

	static int nextpidnumber();
	static int pidnumber;

	//overriden members
	int save_device( char *buff );
	int evaluate();
};

#endif //CIP_TECH_DEF
