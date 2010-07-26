#ifndef PRIEM_H
#define	PRIEM_H

#include "tech_def.h"
#include "PAC_dev.h"
//-----------------------------------------------------------------------------
class my_comb : public tech_object
    {
    public:
        //-Comb modes.
        enum COMB_MODES
            {
            C_WASH,     //����� ����� ������ ���������
            };
        //-End of comb modes.-!>

        //-Comb timers.
        enum COMB_TIMERS
            {
            //Timers N 0-3 - system timers.
            };
        //-End of comb timers.-!>

        //-Comb parameters.
        enum COMB_PARAMS
            {
            };
        //-End of comb parameters.-!>

        int init_mode( u_int mode );
        int evaluate();
        int final_mode( u_int mode );
        my_comb( int stCnt, int parCnt, int workParCnt, int tmrCnt );
        ~my_comb();
    };
//-----------------------------------------------------------------------------
enum CONSTANTS
    {
    C_COMB_INDEX   = 0,

    C_TANK_1_INDEX = 1,
    C_TANK_2_INDEX = 2,
    C_TANK_3_INDEX = 3,
    C_TANK_4_INDEX = 4,
    
    C_POST_1_INDEX = 5,
    C_POST_2_INDEX = 6,

    POST1_FLOW = 5,
    POST2_FLOW = 6,
    };
//-----------------------------------------------------------------------------
//-Tank modes.
enum TANK_MODES
    {
    T_WHEY_ACCEPTING,            //������ ���������
    T_WHEY_OUT_P1,               //������ ��������� ���� 1
    T_WHEY_OUT_P2,               //������ ��������� ���� 2
    T_WASH,                      //�����

    T_WHEY_WACCEPTING = 8,       //�������� ������ ���������
    T_WHEY_WOUT_P1,              //�������� ������ ��������� ���� 1
    T_WHEY_WOUT_P2,              //�������� ������ ��������� ���� 2

    //--������ �����.
    POST_WHEY_HEATING = 16,      //�������� ��������� ������
    POST_WHEY_ACCEPTING,         //������ ��������� ������
    POST_WHEY_ACCEPTING_PAUSE,   //����� ������ ��������� ������
    POST_WHEY_ACCEPTING_END,     //���������� ������ ��������� ������
    //--������ �����.!->

    //--�������.
    RESET_POST = 30,             //��������� �����/������, �������� ������ ���������
    SET_PAUSE_AND_HEATING,       //�������� ����� � ��������
    RESET_TANK_POST1,            //��������� ����� ������ ��� ����� �, ���� ����, ����� 1
    RESET_TANK_POST2,            //��������� ����� ������ ��� ����� �, ���� ����, ����� 2

    SET_POST1_AND_TANK,          //�������� ���� � ���� 1
    SET_POST2_AND_TANK,          //�������� ���� � ���� 2
    SET_HEATING_POST1_AND_TANK,  //�������� ���� � �������� ���� 1
    SET_HEATING_POST2_AND_TANK,  //�������� ���� � �������� ���� 2
    };
//-End of tank modes.!->
//-----------------------------------------------------------------------------
class whey_tank : public tech_object
    {
    public:
        //-Tank parameters.
        enum TANK_PARAMETERS
            {
            T_WARNING_REASON = 4,
            };
        //-End of tank parameters.-!>

        //-Tanks errors.
        enum TANK_WARNINGS
            {
            tankInStop = 1,
            tankInStart = 2,
            };
        //-Tanks errors.-!>

        smart_ptr< i_DO_device > V1;  //�������� ������ �� ������ � ����
        smart_ptr< i_DO_device > V2;  //������ �� ������ � ���� �� ����� ������ ���������
        smart_ptr< i_DO_device > V3;  //������ �� ������
        smart_ptr< i_DO_device > V4;  //�������� ������ �� ������
        smart_ptr< i_DO_device > V5;  //�������� ������ ����� ����� �����

        smart_ptr< i_DO_device > V7_1; //������ �������� ����� ����� 1
        smart_ptr< i_DO_device > V7_2; //������ �������� ����� ����� 2

        smart_ptr< i_DI_device > LSL;
        smart_ptr< i_AI_device > LE1;
        smart_ptr< i_DI_device > LSH;
        smart_ptr< i_AI_device > T;

        //-��� ������ � ���������.
        smart_ptr< i_DO_device > V_1;
        smart_ptr< i_DO_device > V_2;
        smart_ptr< i_DO_device > V_3;
        smart_ptr< i_DO_device > V_4;
        //-��� ������ � ���������.-!>

        int set_mode( u_int mode, int newm );
        int evaluate();
        int final_mode( u_int mode );
        int init_mode( u_int mode );

        whey_tank( int par_count, int n, int tmr_cnt );
        ~whey_tank();
    };
//-----------------------------------------------------------------------------
const int delayTempTmr = 1;
const int pauseTimeTmr = 2;

class post : public tech_object
    {
    public:
        //-Post parameters.
        enum PParameters
            {
            SECTION1 = 1,     //������� ������ 1.
            SECTION2,
            SECTION3,
            SECTION4,
            SECTION5,
            SECTION6,
            SECTION7,
            SECTION8,
            SECTION9,
            SECTION10,
            AUTO_NUM,       //����� ������.
            CURRENT_SEC,    //������� ������.
            SECTIONS,       //����� ����� ������.
            TOT_VOL,        //����� �����.
            IS_RESET_POST,  //15 ��� ��������� ����� �������� �������.

            P_WARNING_REASON = 17,

            T_LE_MIN = 19,  //19 ����������� ������� � �����, ���� �������� ��������� � ���������� ����� �������
            T_OUT_MAX,      //20 ������������ ����������� ������, ��� ���������� ������� ��������� ��������� ��������
            T_OUT_NORMAL,   //21 ����������� ������, ��� ���������� ������� ����������� ������� ������ ���������
            PAUSE_TIME_MAX, //22 ���� ����� ������ �����.
            };
        //-End of post parameters.-!>

        //-Post errors.
        enum postWarnings
            {
            maxTPost = 1,
            noFlowPost = 2,
            pauseTimeLeft,
            };
        //-Post errors.-!>

        smart_ptr< i_DO_device > V1;    //������ ����.
        smart_ptr< i_AI_device > outTE; //����������� ������.

        smart_ptr< i_DO_device > N1; //����� ������.
        smart_ptr< i_counter >   ctr;//�������

        smart_ptr< i_DO_device > lampReady;
        smart_ptr< i_DO_device > lampWorking;
        smart_ptr< i_DI_device > flow;

        smart_ptr< i_DI_device > btnStart;
        smart_ptr< i_DI_device > btnPause; //Start, stop buttons.
        int       btnStartPrevState, btnPausePrevState;

        int       prevSectVol;
        char      shutDown;

        int evaluate();
        int final_mode( u_int mode );
        int init_mode( u_int mode );

        int init_params();

        post( int par_count, int n, int tmr_cnt );
        ~post();
    };
//-----------------------------------------------------------------------------
#endif	// PRIEM_H
