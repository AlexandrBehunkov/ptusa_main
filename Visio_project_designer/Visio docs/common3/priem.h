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
#include "plc.h"
#include "g_device.h"
#include "plc_dev.h"
#include "f_upload.h"
#include "modemngr.h"

#ifndef WATER_CNT
#include "TComb.h"
#endif

#if defined I7188_E || defined I7186_E || defined uPAC5000
#include "tcpCmctr.h"
#endif

#if !defined I7188_E && !defined I7186_E && !defined uPAC5000
#include "cmmctr.h"
#endif

#ifdef USE_SIMPLE_DEV_ERRORS
#include "snprintf.h"
#endif // USE_SIMPLE_DEV_ERRORS

extern DEB;

//timer functions
#define TS(x) TMR[x]->R();TMR[x]->S()
#define TSS(x,y) TMR[x]->SZ(y*1000L);TMR[x]->R();TMR[x]->S()
#define TSMS(x,y) TMR[x]->SZ(y);TMR[x]->R();TMR[x]->S()
#define TSETS(x,y) TMR[x]->SZ(y*1000L);TMR[x]->R()
#define TSETMS(x,y) TMR[x]->SZ(y);TMR[x]->R()
//-timer functions


//==================================================================================
enum OBJ_TYPE
	{
	COMB		= 0,	//	no (0)
	TANK		= 1,	//	no
	TANK_TYPE	= 2,	//	no
	TANK_ALL	= 3,	//	0
	}; 

//	�������� ���������� �������������� ������ ������� 'mode' ������� 'obj'
//		� ����������� ���������� ��������� ������ ��������� ������
//
int Check_Block_Mode( OBJ_TYPE obj, int nom, int mode, int err_par = 10 );
//	obj		:	��� ������� ( COMB, TANK, TANK_TYPE )
//	nom		:	����� ���������������� �������
//	mode	:	����� ������������ ������ (���� �� �������, �� ��������� ������������� �����)
//	err_par	:	�������� � ������� ����� ������������ ���������� � ����������� ������
//
//	res		:	���������� ����� ������� (������������ ������ �������)
//==================================================================================


int IsAnyMode     (int mode, int lastTankN = 100 );
int IsAnyModeC    (int mode1, int mode2);
int IsAnyModeT    ( int mode1, char TankType ); 

TVDEV *DEV( dev_elem dev );

//------------------------------------------------------------------------------
int ProcMainParams( int argc, char *argv[ ] );
//------------------------------------------------------------------------------
/// @brief �������� ���������� �� ������� �������� ������� (����, ��������).

#ifdef USE_SIMPLE_DEV_ERRORS
class error_tech_device
    {
    public:
        /// @brief ��������� �������� ������.
        ///
        /// ���� ������ ���, ������������ ������ ������.
        ///
        /// @return ��������� �������� ������.        
        virtual const char* get_error_str() const = 0;

        /// @brief ��������� ������� ������.
        ///
        /// @return 0   - ��� ������.
        /// @return > 0 - ���� ������.
        virtual int is_exist_error() const = 0;

        /// @brief ����� ������.
        ///
        virtual int reset_error() = 0;

        /// @brief ��������� ���� ������.
        ///
        /// @return ��������� ��� ������.        
        virtual const char* get_type() const = 0;

        /// @brief ��������� ������ ������.
        ///
        /// @return ����� ������.
        virtual int get_error_number() const = 0;

        virtual int get_priority() const = 0;

        enum PRIORITY
            {
            P_ALARM   = 250,
            P_MESSAGE = 500,
            P_ANSWER  = 750,
            };
    };
#endif // USE_SIMPLE_DEV_ERRORS
//------------------------------------------------------------------------------
class TMyComb
#ifdef USE_SIMPLE_DEV_ERRORS
: public error_tech_device
#endif // USE_SIMPLE_DEV_ERRORS
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

    char **mode_name; ///< �������� �������.

    public:   
#if defined I7188_E || defined I7186_E
        i_active_device *active_dev;
#endif // defined I7188_E || defined I7186_E

        //��� �������� ������ � ������.
        unsigned long   cmd;        //�������.
        complex_device  *com_dev;
        //-��� �������� ������ � ������.-!>

        unsigned char	statesCnt;
        int				tmrCnt;
        TParams			*par;
        TTimer			**TMR;
        //int				UseatOn;
        //int				LseatOn;
        unsigned long	*state;
		int             no;

		////	���������� ������� ��� ���������� UPR ��� ���������� �������� ����� � �������.
		//ulong			time_for_off_UPR;
		////	���������� ������� ��� ��������� ������ ��� ��������� ��������� ���������� (�������).
		//ulong			time_for_on_mode;
		////	���������� ������� ��� ���������� ������ ��� ��������� ��������� ���������� (�������).
		//ulong			time_for_off_mode;
        
		static int		CombCnt;
        struct TPathRec	*paths;
        
        //-������ �������.
#ifdef USE_SIMPLE_DEV_ERRORS
        char  err_str[ 150 ];
        uchar is_err;
        int   error_number;
        int   err_type;

        int get_error_number() const 
            {
            return error_number;
            }

        const char* get_error_str() const
            {
            return err_str;
            }

        int is_exist_error() const
            {
            return is_err;
            }

        int reset_error();
       
        enum ERR_MSG_TYPES
            {
            ERR_CANT_ON,
            ERR_ON_WITH_ERRORS,
            ERR_OFF,
            ERR_DURING_WORK,
            ERR_SIMPLE,
            };
        /// @brief ��������� ������.
        ///
        /// � ��������� ����������� ���������� � ������ ��������.
        ///
        /// @param [in] err_msg - �������� ������.
        /// @param [in] mode - ����� ������.
        /// @param [in] type - ��� ���������.
        int set_err_msg( const char *err_msg, int mode, 
            ERR_MSG_TYPES type = ERR_CANT_ON );

        
        const char* get_type() const;
        
        int get_priority() const;

        /// @brief �������� ������������� �������� ��������� �� ������ ��������
        /// ����� ����� ���������� ������.
        ///
        /// @return 0 - �� ���� ���������.
        /// @return 1 - ���� ���������.
        virtual int is_check_mode( int mode ) const;

        const char* get_mode_name( int mode ) const;
        int set_mode_name( int mode, char* name )
            {
            mode_name[ mode ] = name;
            return 0;
            }
#endif // USE_SIMPLE_DEV_ERRORS
        //-������ �������-!>.

        TMyComb( int no, int stCnt, int parCnt, int workParCnt, int tmrCnt );
        ~TMyComb();

		virtual void Key_Work( int i );

        virtual int SetMode   ( int mode, int newm );
        inline  int GetMode   ( int mode ) const;
        virtual int Evaluate  ();
        virtual int FinalMode ( int mode );
        virtual int InitMode  ( int mode );  

#ifdef USE_SIMPLE_DEV_ERRORS
        virtual int is_any_mode() const;
#endif // USE_SIMPLE_DEV_ERRORS

        /// @brief ������������� ��������� ��������� ���������� � ������, 
        /// � ��������� - 0.
        virtual int InitParams();  
        virtual int InitWorkParams();

		TVDEV* key_btn_Drain;
		TVDEV* key_btn_Tank;
		TVDEV* key_lamp;
    };
//---------------------
class TTank 
#ifdef USE_SIMPLE_DEV_ERRORS
: public error_tech_device
#endif // USE_SIMPLE_DEV_ERRORS
    {
	enum TankParams
		{
		//-������� ���������.
		TANK_ERRORS,			//	0	����� ������ � ��������� ��� �����
		PROCESS_TIME,			//	1	����� �������� ������   
		STEP_NUMBER,			//	2	����� �������� ����
		PRODUCT_TYPE,			//	3	��� ��������
		//	4	
		CROS_PROC_TIME = 5,		//	5	����� ����������� ��������, ���� (3000)

		//-������� ���������.-!> 
		};

	enum TankTimers //Comb standard timers.
		{
		TMR_OFF_MODE = 0,		//	������ ��� ���������� ������ ����� ������������ ���������� ������
		};

	ulong start_cross_period;	//	����� ������� ������� �� ���������� ������ (������������ � ������� ��������)
	
	char	Prev_LH;			//  ���������� ��������� �������� ������
	char	Prev_LL;			//  ���������� ��������� ������� ������


    public:
        unsigned long   modeStartTime[33];
        unsigned long   modeEvaluationTime[32];

        //������ � ������ �������.
        mode_manager    *modes_manager;

        int             tmrCnt; //���������� ��������.
    protected:
        char            isModeEvaluationTimeLeft[32];        

    public:  
#if defined I7188_E || defined I7186_E
        i_active_device *active_dev;
#endif // defined I7188_E || defined I7186_E

        TParams         *par;
        unsigned long   state;
        TTimer          **TMR;
        int             no;
		int				mode_cnt;
        char            PodTip;     //  ������ �����

        //-��� �������� ������ � ������.
        unsigned long   cmd;                //�������.
        complex_device  *com_dev;
        //-��� �������� ������ � ������.-!>

        static int		TankCnt;			//���������� ������;
        int				comb_n;				//����� ��������, � ������� ����������� ����;
        
		struct TPathRec	*paths;

		//-������ �������.
#ifdef USE_SIMPLE_DEV_ERRORS
		char  err_str[ 200 ];
        uchar is_err;
        int   error_number;
        int   err_type;
        
        char  *mode_name[ 32 ];

        int get_error_number() const 
            {
            return error_number;
            }

        const char* get_error_str() const
            {
            return err_str;
            }

        int is_exist_error() const
            {
            return is_err;
            }

        int reset_error();

        const char* get_type() const;    

        int get_priority() const;

        /// @brief �������� ������������� �������� ��������� �� ������ ��������
        /// ����� ����� ���������� ������.
        ///
        /// @return 0 - �� ���� ���������.
        /// @return 1 - ���� ���������.
        virtual int is_check_mode( int mode ) const;

        const char* get_mode_name  ( int mode ) const;

        enum ERR_MSG_TYPES
            {
            ERR_CANT_ON,
            ERR_ON_WITH_ERRORS,
            ERR_OFF,
            ERR_DURING_WORK,
            ERR_SIMPLE,
            };

        int set_err_msg( const char *err_msg, int mode, 
            ERR_MSG_TYPES type = ERR_CANT_ON );

        int set_mode_name( int mode, char* name );
#endif // USE_SIMPLE_DEV_ERRORS
        //-������ �������-!>.

    public:
        TTank ( TParams *tpar, int n, int tmrCnt );
        TTank ();
        ~TTank();

        int         SetModeTime   ( int mode, unsigned long modeTime );
        virtual int SetMode       ( int mode, int newm );
        int         GetMode       ( int mode ); 

#ifdef USE_SIMPLE_DEV_ERRORS
		int GetModeEx( int mode, ... );

		void init_levels( TVDI*	LL, TVDI* LH );

        virtual int is_any_mode() const
            {
            return state > 0 ? 1 : 0;
            }
#endif // USE_SIMPLE_DEV_ERRORS

        virtual int InitMode  ( int mode );
		virtual int Evaluate  ();
        virtual int FinalMode ( int mode );
        
		void from_wait_to_work( int i );	//	������������ ������ �� �������� � ������ (�������� ����� � �.�.)
		void Key_Work( int i );

        virtual int InitParams();  //�������������� ��������� � ������ 0.
        virtual int InitWorkParams();


        //��� ����������/�������� ����������.
        virtual int save_active_state( char *buff ); 
        virtual int load_active_state( char *buff ); 
        virtual int set_default_state();
        virtual int get_saved_size (); 

        private:
            char is_lvl_err; //������� ������ �������.

            TVDI*	LL;						//	������ �������
            TVDI*	LH;						//	������� �������

			TVDEV* key_btn_Drain;
			TVDEV* key_btn_Tank;
			TVDEV* key_lamp;
    };
#endif //PRIEM_H

