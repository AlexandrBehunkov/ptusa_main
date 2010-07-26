/// @file prj_mngr.h
/// @brief ������ � ��������� ������������ �������, ������������� �������� � 
/// �.�.
/// 
/// @author  ������ ������� ���������.
///
/// @par ������� ������:
/// @$Rev: 54 $.\n
/// @$Author: id@BMK $.\n
/// @$Date:: 2010-04-26 15:27:20#$.

#ifndef PRJ_MANAGER_H
#define PRJ_MANAGER_H

#include "sys.h"
//-----------------------------------------------------------------------------
/// @brief ������������� �������.
///
/// 
class project_manager
    {
    public:
        
        /// @brief ��������� ���������� ��������� ������.
        /// 
        /// @param argc - ���������� ����������.
        /// @param argv - ������ ����������.
        int proc_main_params( int argc, char *argv[] );

        /// @brief �������� ������������ �������.
        /// 
        /// @param file_name - ��� ����������������� ����� �������.
        int load_configuration( const char *file_name );

        /// @brief ��������� ������������� ���������� ������.
        static project_manager* get_instance();

        /// @brief ��������� ������������� ���������� ������.
        static void set_instance( project_manager* new_instance );

    protected:
        file *cfg_file;     ///< ���������������� ����.

    private:
        static project_manager* instance;   ///< ������������ ��������� ������.
    };
//-----------------------------------------------------------------------------
#define G_PROJECT_MANAGER project_manager::get_instance()
//-----------------------------------------------------------------------------
#endif // PRJ_MANAGER_H
