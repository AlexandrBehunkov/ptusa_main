//����� ��������.
//  �������� �������� �������, ������� ������������ ��� ������������� 
//  ������� ���������������� ��������� �������� ���������������� �������� - 
//  ���� (��������������� �������), �������� (������������ ��������).
//�������� DEFINE.
//  I7188_E - ���������� ���������� ��� ����������� I7188E.
//  I7186_E - ���������� ���������� ��� ����������� I7186E.
//  NO_TANKS_MODE - �� ������������ 32 ������ �����, � ������ 1. �������
//      ��� ����������� ������.

#ifndef PRIEM_H
#define PRIEM_H

#include <stdio.h>

#include "sys.h"
#include "g_device.h"
#include "PAC_dev.h"
#include "PAC_err.h"

#include "tcp_cmctr.h"
#include "param_ex.h"

//timer functions
#define TS(x) TMR[x]->R();TMR[x]->S()
#define TSS(x,y) TMR[x]->SZ(y*1000L);TMR[x]->R();TMR[x]->S()
#define TSMS(x,y) TMR[x]->SZ(y);TMR[x]->R();TMR[x]->S()
#define TSETS(x,y) TMR[x]->SZ(y*1000L);TMR[x]->R()
#define TSETMS(x,y) TMR[x]->SZ(y);TMR[x]->R()
//-timer functions

int IsAnyMode     (int mode, int lastTankN = 100 );
int IsAnyModeC    (int mode1, int mode2);
int IsAnyModeT    ( int mode1, char TankType ); 
                                
//---------------------
int ProcMainParams( int argc, char *argv[ ] );
//---------------------
class TMyComb
    {  
    enum combTimers //Comb standard timers.
        {
        TMR_USEATO,
        TMR_USEATC,
        TMR_LSEATO,
        TMR_LSEATC,
        };

    enum combParams //Comb standard parameters.
        {
        P_SEAT_PERIOD = 15,
        P_SEAT_TIME,
        };

    public:   
#if defined I7188_E || defined I7186_E
        i_active_device *active_dev;
#endif // defined I7188_E || defined I7186_E

        //��� �������� ������ � ������.
        u_int_4         cmd;        //�������.
        complex_device  *com_dev;
        //-��� �������� ������ � ������.-!>

        unsigned char       statesCnt;
        int                 tmrCnt;
        saved_params_float      *par;
        run_time_params_float   *wpar;

        timer   **TMR;
        int	UseatOn;
        int	LseatOn;
        u_int_4 *state;

        static int		CombCnt;
        //struct TPathRec	*paths;
        
        TMyComb( int stCnt, int parCnt, int workParCnt, int tmrCnt );
        virtual ~TMyComb();

        int         SetMode   ( int mode, int newm );
        inline  int GetMode   ( int mode );
        virtual int Evaluate  ();
        virtual int FinalMode ( int mode );
        virtual int InitMode  ( int mode );  

        virtual int InitParams();  //�������������� ��������� ��������� ��������� � ������, ��������� - 0.
        virtual int InitWorkParams();
    };
//---------------------
class TTank
    {
    public:
        unsigned long   modeStartTime[33];
        unsigned long   modeEvaluationTime[32];

        //������ � ������ �������.
        //mode_manager    *modes_manager;

        int             tmrCnt; //���������� ��������.
    protected:
        char            isModeEvaluationTimeLeft[32];        

    public:  
#if defined I7188_E || defined I7186_E
        i_active_device *active_dev;
#endif // defined I7188_E || defined I7186_E

        int                 no;
        
        saved_params_float  *par;
        u_int_4             state;
        timer               **TMR;
        
        char                PodTip;     //  ������ �����

        //-��� �������� ������ � ������.
        u_int_4         cmd;                //�������.
        complex_device  *com_dev;
        //-��� �������� ������ � ������.-!>

        static int  TankCnt;			//���������� ������;
        int     comb_n;				//����� ��������, � ������� ����������� ����;
        
    public:
        TTank ( saved_params_float *tpar, int n, int tmrCnt );
        TTank ();
        virtual ~TTank();

        int         SetModeTime   ( int mode, unsigned long modeTime );
        virtual int SetMode       ( int mode, int newm );
        int         GetMode       ( int mode );  

        virtual int Evaluate  ();
        virtual int FinalMode ( int mode );
        virtual int InitMode  ( int mode );

        virtual int InitParams();  //�������������� ��������� � ������ 0.
        virtual int InitWorkParams();

        //��� ����������/�������� ����������.
        virtual int save_active_state( char *buff ); 
        virtual int load_active_state( char *buff ); 
        virtual int set_default_state();
        virtual int get_saved_size (); 
    };
#endif //PRIEM_H

