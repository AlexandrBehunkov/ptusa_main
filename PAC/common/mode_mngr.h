/// @file mode_mngr.h
/// @brief �������� �������� �������, ������� ������������ ��� ����������� �����
/// ��� ���������� ������� �����, ��������.
/// 
/// ����� @ref step_path, �������� ��� ����������� ������ � �����, �������� ���  
/// ����������� ���������� - ������ �����������\����������� ���������,  
/// ��������� ����, ��������� � ��� ������� ��������. ��� - ������������ ���  
/// ���������� ���������������� ��������, ���������\���������� ��������  
/// ���������� �������������, ��� ����������������� ������� ���������. � 
/// ������� �� ������, ������� �������� ��������\��������� ���. ��� �������� 
/// ���� ������� ������� ������ ����� @ref mode_manager. 
/// 
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// 
/// @par ������� ������:
/// @$Rev: 1631 $.\n
/// @$Author: id $.\n
/// @$Date:: 2010-03-06 14:43:44#$.

#ifndef MODE_MNGR 
#define MODE_MNGR 

#include "PAC_dev.h"
#include "param_ex.h"

//-----------------------------------------------------------------------------
/// @brief �������� ���������� �� �����������, ������� ������ � ��� (�����������/
/// �����������).
///
/// � ������ ����� ���� �������� (�����������) ������ ���� ���.
class step_path
    {
    public:
        step_path();

        /// @brief ���������� ����������, ������� �����������\����������� ��
        ///  ����� ���������� ����.
        ///
        /// @param [in] dev - ��������� �� ����������� ����������.        
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_closed_dev( device *dev );

        /// @brief ���������� ����������, ������� ����������\����������� �� 
        /// ����� ���������� ����.
        ///
        /// @param [in] dev - ��������� �� ����������� ����������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_opened_dev( device *dev );

        int init();
        int evaluate() const;
        int final( );

        /// ��������� ������� ������ ����.
        inline u_int_4 get_start_time() const;

        /// ������������ ������� ������ ����.
        inline void set_start_time( u_int_4 start_time );

        /// ������� �� ������� ������.
        void print() const;

    private: 
        std::vector< device* > close_devices;  ///< ����������� ����������.   
        std::vector< device* > open_devices;   ///< ����������� ����������.    
        
        u_int_4 start_time;                   ///< ����� ������ ����.
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� � ���� ����� ������-���� ������� (����, 
/// ��������).
/// 
/// � ������� (����, ...) ����� ���� �������� ����������� ��������� �������.
class mode_manager
    {
    public:
        saved_params_u_int_4 *par;       ///< ���������, ���������� ����������������� �����.

        mode_manager( u_int_2 new_modes_cnt );

        /// ���������� ��������� � ��������� �����.
        int set_param( saved_params_u_int_4 *par );

        /// @brief ������������� ���������� ����� ��� ������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int set_mode_config( u_int_2 mode, u_char new_steps_cnt );

        int init( u_int_2 mode, u_char start_step = 0, void *object = 0 );
        int evaluate( u_int_2 mode );
        int final( u_int_2 mode );

        /// @brief ������� � ����.
        ///
        /// @param [in] mode     - �����.
        /// @param [in] new_step - ���, �������� 0..255.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int to_step( u_int_2 mode, u_char new_step );


        /// @brief ���������� ����������� ���������� ��� ����.
        ///
        /// @param [in] mode - �����;
        /// @param [in] step - ���, �������� 0..255.
        /// @param [in] dev  - ��������� �� ����������� ����������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_closed_dev( u_int_2 mode, u_char step, device *dev );

        /// @brief ���������� ����������� ���������� ��� ����.
        ///
        /// @param [in] mode - �����.
        /// @param [in] step - ���, �������� 0..255.
        /// @param [in] dev  - ��������� �� ����������� ����������.       
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_opened_dev( u_int_2 mode, u_char step, device *dev );

        /// @brief �������� ������� ���������� ����.
        ///
        /// @return  < 0 - ������.
        /// @return    0 - ����� �� �����.
        /// @return    1 - ����� �����.
        int is_current_step_evaluation_time_left( u_int_2 mode );

        /// @brief ����� ���������� ����.
        ///
        /// @return    1 - ����� ���������� ����.
        unsigned long get_current_step_evaluation_time( u_int_2 mode );

        int get_active_step( u_int_2 mode );

        void print();

    private:
        u_char  **step_duration_par_n;  ///< ������ ���������� ������� ����.    
        u_char  **next_step_n;          ///< ������ �����, � ������� ������� ���  
                                        ///< ���������� ������� �����.

        u_int_2 modes_cnt;              ///< ���������� �������.
        u_char  *steps_cnt;             ///< ���������� �����.

        step_path **steps;              ///< ���� ��� ������� ������.

        u_char  *active_step;           ///< ����, ����������� � ������� ������.
        u_char  *is_active_mode;        ///< ������� �� �����.

        /// @brief ��������, �������� �� ����� ������ � ���� ����������.
        ///
        /// @return -2 - ����� ������ ������� �� �������.
        /// @return -1 - ����� ���� ������� �� �������.
        /// @return  0 - �k.
        int check_correct_step_n( u_int_2 mode, u_char step );
    };
//-----------------------------------------------------------------------------
#endif // MODE_MNGR
