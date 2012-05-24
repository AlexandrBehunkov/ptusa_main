/// @file modeMngr.h
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
/// @c PAC - ���������� ��� �����������.@n
/// @c WIN - ���������� ��� Windows.
/// 
/// @par ������� ������:
/// @$Rev: 1631 $.\n
/// @$Author: id $.\n
/// @$Date:: 2012-01-12 13:16:57#$.

#ifndef MODEMNGR 
#define MODEMNGR 

#include "plc.h"

//-----------------------------------------------------------------------------
/// @brief �������� ���������� �� �����������, ������� ������ � ��� (�����������/
/// �����������).
///
/// � ������ ����� ���� �������� (�����������) ������ ���� ���.
class step_path
    {
    public:
        step_path( u_int_2 new_close_dev_cnt = 0, 
            u_int_2 new_open_device_cnt = 0 );

        /// ������� ��������� �������� ��� ����.
        void set_path( char x1, char y1, char x2, char y2 );

        /// @brief ��������� ���������� �����������\����������� ���������.
        ///        
        /// @param new_close_dev_cnt - ����� ����������� ���������.
        /// @param new_open_dev_cnt  - ����� ����������� ���������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int set_dev_cnt( u_int_2 new_close_dev_cnt, u_int_2 new_open_dev_cnt );

        /// @brief ���������� ����������, ������� �����������\����������� ��
        ///  ����� ���������� ����.
        ///
        /// @param [in] dev - ��������� �� ����������� ����������.        
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_closed_dev( TVDO *dev );

        /// @brief ���������� ����������, ������� ����������\����������� �� 
        /// ����� ���������� ����.
        ///
        /// @param [in] dev - ��������� �� ����������� ����������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_opened_dev( TVDO *dev );

        int init( void* object = 0, int mode = 0 );
        int evaluate() const;
        int final( );

#ifdef USE_SIMPLE_DEV_ERRORS
        /// @brief �������� �� ������� ������ �������� ����� ���������, �������
        /// ����������\����������� (�����������\�����������) �� ����� ����������
        /// ����.
        ///
        /// @param [out] err_dev_name - ����� ��������� ���������.
        ///
        /// @return > 0 - ���� ����� ����������.
        /// @return   0 - ��, ����� ��������� ���.
        int check_devices( char* err_dev_name, int max_to_write ) const;
#endif // USE_SIMPLE_DEV_ERRORS

        /// ���������� ����� ������ ����.
        inline u_int_4 get_start_time() const;

        /// ������������� ����� ������ ����.
        inline void set_start_time( u_int_4 start_time );

        /// ������� �� ������� ������.
        void print() const;

    private:
        //  [ 1 ]
        /// @var close_devices_cnt
        /// @brief ���������� ����������� ���������.         
        /// @details ��� �������� ���������� ������ ������, � �������  
        /// �������� ��������� �� ����������� ����������. 
        //  [ 2 ]
        /// @var open_devices_cnt
        /// @brief ���������� ����������� ���������.
        /// @details ��� �������� ���������� ������ ������, � �������  
        /// �������� ��������� �� ����������� ����������.
        //  [ 3 ]
        /// @var last_close_dev
        /// @brief ����� ���������� ������������ ����������. 
        //  [ 4 ]
        /// @var last_open_dev
        /// @brief ����� ���������� ������������ ����������.
        //  [ 5 ]
        /// @var closeDevices
        /// @brief ����������� ����������. 
        //  [ 6 ]
        /// @var openDevices
        /// @brief ����������� ����������.
        u_int_2 close_devices_cnt;  // 1   
        u_int_2 open_devices_cnt;   // 2
        u_int_2 last_close_dev;     // 3        
        u_int_2 last_open_dev;      // 4 
        TVDO    **closeDevices;     // 5    
        TVDO    **openDevices;      // 6       
        
        u_int_4 start_time;         ///< ����� ������ ����.

        char    in_x;               ///< ���������� x ������ ��������.
        char    in_y;               ///< ���������� y ������ ��������.
        char    out_x;              ///< ���������� x ����� ��������.
        char    out_y;              ///< ���������� y ����� ��������.
    };
//-----------------------------------------------------------------------------
/// @brief �������� ���������� � ���� ����� ������-���� ������� (����, 
/// ��������).
/// 
/// � ������� (����,...) ����� ���� �������� ����������� ��������� �������.
class mode_manager
    {
    public:
        TParams *par;       ///< ���������, ���������� ����������������� �����.

        mode_manager( u_int_2 new_modes_cnt );

        /// ��������� ��������� � ��������� �����.
        int set_param( TParams *par );

        /// @brief ������������� ���������� ����� ��� ������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int set_mode_config( u_int_2 mode, u_char new_steps_cnt );

        int init( u_int_2 mode, u_char start_step = 0, void *object = 0 );
        int evaluate( u_int_2 mode );
        int final( u_int_2 mode );

#ifdef USE_SIMPLE_DEV_ERRORS
        /// @brief �������� �� ������� ������ �������� ����� ���������, �������
        /// ����������\����������� (�����������\�����������) �� ����� ����������
        /// ���� ����� ������.
        ///
        /// @param [in]  mode - ����� ������.
        /// @param [out] err_dev_name - ����� ��������� ����������.
        ///
        /// @return > 0 - ���� ����� ����������.
        /// @return   0 - ��, ����� ��������� ���.
        int check_mode_devices( u_int_2 mode, char* err_dev_name, int str_len );
#endif // USE_SIMPLE_DEV_ERRORS

        /// @brief ������� � ����.
        ///
        /// @param [in] mode     - �����.
        /// @param [in] new_step - ���, �������� 0..255.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int to_step( u_int_2 mode, u_char new_step );

        /// @brief ������������� ��������� ����.
        ///
        /// @param [in] mode            - �����.
        /// @param [in] step            - ���, �������� 0..255.
        /// @param [in] close_dev_cnt   - ���������� ����������� ���������.
        /// @param [in] open_device_cnt - ���������� ����������� ���������.
        /// @param [in] next_step       - ����� ����, � ���������� ��������  
        ///                               ������� ��� ���������� �������  
        ///                               �������� ����.
        /// @param [in] par_n           - ����� ���������, ����������� ����� 
        ///                               ���� � ���.
        /// @param [in] x1              - ���������� x ������ �������� ��������.
        /// @param [in] y1              - ���������� y ������ �������� ��������.
        /// @param [in] x2              - ���������� x ����� �������� ��������.
        /// @param [in] y2              - ���������� x ����� �������� ��������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int set_step_config( u_int_2 mode, u_char step, u_int_2 close_dev_cnt, 
            u_int_2 open_device_cnt, u_char next_step = 0,
            u_char par_n = 0, u_char x1 = 0, u_char y1 = 0,
            u_char x2 = 0, u_char y2 = 0 );

        /// @brief ��������� ����������� ���������� ��� ����.
        ///
        /// @param [in] mode - �����;
        /// @param [in] step - ���, �������� 0..255.
        /// @param [in] dev  - ��������� �� ����������� ����������.
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_closed_dev( u_int_2 mode, u_char step, TVDO *dev );

        /// @brief ��������� ����������� ���������� ��� ����.
        ///
        /// @param [in] mode - �����.
        /// @param [in] step - ���, �������� 0..255.
        /// @param [in] dev  - ��������� �� ����������� ����������.       
        ///
        /// @return < 0 - ������.
        /// @return   0 - ��.
        int add_opened_dev( u_int_2 mode, u_char step, TVDO *dev );

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

        int save( char *buff );
        int load( char *buff );

        int get_saved_size();
        int get_active_step( u_int_2 mode );


        void print();
			
		u_char  *steps_cnt;             ///< ���������� �����.	
				
    private:
        u_char  **step_duration_par_n;  ///< ������ ���������� ������� ����.    
        u_char  **next_step_n;          ///< ������ �����, � ������� ������� ���  
                                        ///< ���������� ������� �����.

        u_int_2 modes_cnt;              ///< ���������� �������.
        //u_char  *steps_cnt;             ///< ���������� �����.
		step_path **steps;              ///< ���� ��� ������� ������.
		        
        u_char  *active_step;           ///< ����, ����������� � ������� ������.
        u_char  *is_active_mode;        ///< ������� �� �����.

        /// @brief ���������, �������� �� ����� ������ � ���� ����������.
        ///
        /// @return -2 - ����� ������ ������� �� �������.
        /// @return -1 - ����� ���� ������� �� �������.
        /// @return  0 - �k.
        int check_correct_step_n( u_int_2 mode, u_char step );
    };
//-----------------------------------------------------------------------------
#endif // MODEMNGR