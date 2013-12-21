/// @file mode_mngr.h
/// @brief �������� �������� �������, ������� ������������ ��� ����������� �����
/// ��� ���������� ������� �����, ��������.
///
/// ����� @ref step, �������� ��� ����������� ������ � �����, �������� ���
/// ����������� ���������� - ������ �����������\����������� ���������,
/// ��������� ����, ��������� � ��� ������� ��������. ��� - ������������ ���
/// ���������� ���������������� ��������, ���������\���������� ��������
/// ���������� �������������, ��� ����������������� ������� ���������. �
/// ������� �� ������, ������� �������� ��������\��������� ���. ��� ��������
/// ���� ������� ������� ������ ����� @ref mode_manager.
/// ����� @ref action �������� ������ ��������� � ��������� ��� ����
/// ������������ ��������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef MODE_MNGR
#define MODE_MNGR

#include <string>

#include "dtime.h"

#include "PAC_dev.h"
#include "PAC_info.h"
#include "param_ex.h"

class mode_manager;
//-----------------------------------------------------------------------------
/// @brief �������� ��� ������������ (���������, ���������� � �.�.).
class action
    {
    public:
        action( std::string name, u_int group_cnt = 1 );

        virtual ~action()
            {
            }

        /// @brief �������� �� ���������� ���������.
        ///
        /// @return true  ���� ����������, ��� �������� ���-�� ��������.
        /// @return false ��� ���������, ��� �������� ���-�� ��������.
        virtual bool is_empty() const;

        virtual void print( const char* prefix = "" ) const;

        /// @brief �������� ��������.
        ///
        /// @param [out] reason ���������, ������ ������ ��������� ��������.
        virtual int check( char* reason ) const
            {
            reason[ 0 ] = 0;
            return 0;
            }

        /// @brief ������������� ��������.
        virtual void init() {}

        /// @brief ���������� ��������.
        ///
        /// @return ��������� ���������� ��������. ���������� � ����������� ��
        /// ��������.
        virtual void evaluate() {}

        /// @brief ���������� ��������.
        virtual void final();

        /// @brief ���������� ���������� � ��������.
        ///
        /// @param [in] dev ����������.
        /// @param [in] group ������ ����������.
        virtual void add_dev( device *dev, u_int group = 0 );

        /// @brief ���������� ���������� � ��������.
        ///
        /// @param [in] dev ����������.
        /// @param [in] group �������������� ��������.
        /// @param [in] subgroup �������������� ��������.
        virtual void add_dev( device *dev, u_int group, u_int subgroup ) {}

        /// @brief ����� ���������� � �����-���� �������.
        ///
        /// @param [out] err_dev_name ����������.
        /// @param [in] max_to_write �������������� ��������.
        /// @return 0 - ��� ������, 1 - ���� ������.
        int check_devices( char* err_dev_name, int max_to_write ) const;

    protected:
        std::vector< std::vector< device* > > devices;  ///< ����������.
        std::string name;                               ///< ��� ��������.
    };
//-----------------------------------------------------------------------------
/// <summary>
/// ��������� ���������.
/// </summary>
class on_action: public action
    {
    public:
        on_action(): action( "��������" )
            {
            }

        void evaluate();
    };
//-----------------------------------------------------------------------------
/// <summary>
/// ���������� ���������.
/// </summary>
class off_action: public action
    {
    public:
        off_action(): action( "���������" )
            {
            }

        void evaluate();
    };
//-----------------------------------------------------------------------------
/// <summary>
/// �������� �����.
/// </summary>
class open_seat_action: public action
    {
    public:
        open_seat_action();

        void init();
        void evaluate();
        void final();

        /// @brief ���������� ���������� � ��������.
        ///
        /// @param [in] dev ����������.
        /// @param [in] group �������������� ��������.
        /// @param [in] seat_type �������������� ��������.
        void add_dev( device *dev, u_int group, u_int seat_type );

        void print( const char* prefix = "" ) const;

        bool is_empty() const;
    private:
        enum PHASES
            {
            P_WAIT = 0,
            P_OPEN_UPPER,
            P_OPEN_LOWER,
            };

        PHASES phase;      ///< ������� ����.
        PHASES next_phase; ///< ��������� ����.

        u_int     active_group_n;  ///< ����� ����������� ������ ������.

        u_int_4 wait_time;      ///< ����� �������� ����� ����������� �����.
        u_int_4 wait_seat_time; ///< ����� �������� ����� ����������� ����� ������.
        u_int_4 wash_time;      ///< ����� �������� ����� ������� ������ ��������.

        /// �����.
        std::vector< std::vector< device* > > wash_upper_seat_devices;
        std::vector< std::vector< device* > > wash_lower_seat_devices;

        u_int_4 start_cycle_time; ///< ����� ������ ����� (�������� ��� ��������).
    };
//-----------------------------------------------------------------------------
/// <summary>
/// ���� DI->DO.
/// </summary>
class DI_DO_action: public action
    {
    public:
        DI_DO_action( ):action( "������ DI->DO DO ..." )
            {
            }

        void evaluate();

        void print( const char* prefix = "" ) const;
    };
//-----------------------------------------------------------------------------
/// <summary>
/// �������� ���������� ������� ��������.
/// </summary>
class required_DI_action: public action
    {
    public:
        required_DI_action(): action( "������� ��� ���������" )
            {
            }

        int check( char* reason ) const;

        void final()
            {
            }
    };
//-----------------------------------------------------------------------------
/// <summary>
/// ����� �����.
/// </summary>
class wash_action: public action
    {
    public:
        wash_action(): action( "�����", 3 )
            {
            }

        void final();

        void evaluate();

        void print( const char* prefix /*= "" */ ) const;

    private:
        enum GROUPS
            {
            G_DI = 0, //������� ������� ������� ��������� ���������.
            G_DO,     //�������� ������� "����� ��".
            G_DEV,    //����������, ���������� �� �������.
            };
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� �� �����������, ������� ������ � ��� (�����������/
/// �����������).
///
/// � ������ ����� ���� �������� (�����������) ������ ���� ���.
class step
    {
    public:
        enum ACTIONS
            {
            A_ON = 0,
            A_OFF,
            A_UPPER_SEATS_ON,
            A_LOWER_SEATS_ON = A_UPPER_SEATS_ON,

            A_REQUIRED_FB,
            A_DO_DI,
            A_WASH,
            };

        step( std::string name, bool is_mode = false );

        ~step();

        /// @brief ��������� �������� ����� �������� ��������������.
        ///
        /// @param idx - ������ ��������.
        ///
        /// @return - �������� �������� � �������� ��������. ���� ������
        /// ������� �� ��������, ������������ �������� 0.
        action* operator[] ( int idx );

        /// @brief �������� ����������� ���������� ����.
        ///
        /// @return > 0 - ������ ���������.
        /// @return   0 - ��.
        int check( char* reason ) const;

        void init();

        void evaluate() const;

        void evaluate_on() const 
            {
            actions[ A_ON ]->evaluate();
            actions[ A_LOWER_SEATS_ON /*A_UPPER_SEATS_ON*/ ]->evaluate();
            };

        void evaluate_off() const 
            {
            actions[ A_OFF ]->evaluate();            
            };

        void final() const;

        /// ��������� ������� ������ ����.
        u_int_4 get_start_time() const;

        /// ������������ ������� ������ ����.
        void set_start_time( u_int_4 start_time );

        /// ������� �� ������� ������.
        void print( const char* prefix = "" ) const;

        /// @brief �������� �� ���������� ���������.
        ///
        /// @return true  ���� ����������, ��� �������� ���-�� ��������.
        /// @return false ��� ���������, ��� �������� ���-�� ��������.
        bool is_empty() const;

        int check_devices( char* err_dev_name, int str_len );

    private:
        std::vector< action* > actions; ///< ��������.
        action action_stub;             ///< ��������� ��������.
        u_int_4 start_time;             ///< ����� ������ ����.

        bool is_mode;     ///< ����������� �� ��� ����� �� ����� ������.
        std::string name; ///< ���.
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� � ������, ��������� �� �����.
///
/// � ������� (����, ...) ����� ���� �������� ����������� ��������� �������.
class mode
    {
    public:
        mode( const char* name, mode_manager *owner, int n );

        step* add_step( const char* name, u_int next_step_n,
            u_int step_duration_par_n );

        /// @brief ��������� ������ ��������� �� �������� ����������� ������������ �����.
        void set_step_cooperate_time_par_n( int step_cooperate_time_par_n )
            {
            this->step_cooperate_time_par_n = step_cooperate_time_par_n;
            }

        /// @brief ��������� ������ ����� �������� ��������������.
        ///
        /// @param idx - ������ ������.
        ///
        /// @return - �������� ������ � �������� ��������. ���� ������
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// mode::step_stub.
        step* operator[] ( int idx );

        int check_on( char* reason ) const;

        void init( u_int start_step = 1 );

        void evaluate();

        void final();

        void to_step( u_int new_step, u_long cooperative_time = 0 );

        u_long evaluation_time()
            {
            return get_delta_millisec( start_time );
            }

        u_long active_step_evaluation_time() const
            {
            if ( active_step_n >= 0 )
                {
                return get_delta_millisec( steps[ active_step_n ]->get_start_time() );
                }

            return 0;
            }

        u_int active_step() const
            {
            return active_step_n + 1;
            }

        /// ������� �� ������� ������.
        void print( const char* prefix = "" ) const;

        const char* get_name() const
            {
            return name.c_str();
            }

        int check_devices( char* err_dev_name, int str_len );
    private:
        std::string name;
        std::vector< step* > steps;

        step* mode_step;

        int active_step_n;           ///< �������� ���.
        int active_step_time;        ///< ����� ��������� ����.
        int active_step_next_step_n; ///< ��������� ���.

        int active_step_second_n;             ///< ������������ �������� ���.
        int active_step_second_start_time;    ///< ������������ �������� ���.
        
        /// @brief ����� ����������� ������� �����.
        u_int step_cooperate_time;

        /// @brief ����� ��������� �� �������� ����������� ������� �����.
        int step_cooperate_time_par_n;

        /// @brief ������ ���������� ������ �����.
        std::vector< int > step_duration_par_ns;

        /// @brief ��������� ����.
        std::vector< int > next_step_ns;

        u_int_4 start_time; ///< ����� ������ ������.
        step step_stub;     ///< ���-��������.

        mode_manager *owner;
        int n;          ///< �����.
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� � ���� ������� ������-���� ������� (����,
/// ��������).
///
/// � ������� (����, ...) ����� ���� �������� ����������� ��������� �������.
class mode_manager
    {
    public:
        mode_manager( u_int modes_cnt );

        ~mode_manager();

        mode* add_mode( const char* name );

        void set_param( saved_params_u_int_4 *par );

        saved_params_u_int_4* get_param() const;

        /// @brief ��������� ������ ����� �������� ��������������.
        ///
        /// @param idx - ������ ������.
        ///
        /// @return - �������� ������ � �������� ��������. ���� ������
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// mode_manager::mode_stub.
        mode* operator[] ( unsigned int idx );

        /// @brief ����� ����������� (��� ���������� �������).
        ///
        /// @return - ����� ������� ��� �������� �������.
        unsigned long get_idle_time();

        /// @brief ���������� ����� ������� � �������.
        void print();

    private:
        /// @brief ���������, ���������� ����������������� �����, �������.
        saved_params_u_int_4 *par;

        std::vector< mode* > modes; ///< ������.

        mode *mode_stub;            ///< �����-��������.

        u_int_4 last_action_time;   ///����� ���������� ���/���� ������.

        static const char* UNKN_MODE_NAME; //��� ��� "������������" ������.
    };
//-----------------------------------------------------------------------------
#endif // MODE_MNGR
