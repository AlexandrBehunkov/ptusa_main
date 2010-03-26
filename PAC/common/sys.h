﻿/// @file sys.h
/// @brief Содержит описания классов, которые реализуют аппаратную часть PAC.
/// 
/// @author  Иванюк Дмитрий Сергеевич.
///
/// @par Описание директив препроцессора:
/// @c LINUX - компиляция для OC Linux.@n
/// @c WIN   - компиляция для Windows.
/// 
/// @par Текущая версия:
/// @$Rev: $.\n
/// @$Author: $.\n
/// @$Date::                    #$.
#ifndef SYS_H
#define SYS_H

typedef unsigned char   u_char;
typedef unsigned int    u_int;

#if defined LINUX || defined WIN
#define Print printf
#define Getch getch
#endif // LINUX

//-----------------------------------------------------------------------------
/// @brief Интерфейс доступа к памяти.
class i_memory
    {
    public:
        /// @brief Чтение массива байт.
        ///        
        /// @param buf       - адрес буфера, куда будут считываться данные.
        /// @param count     - количество считываемых байт.
        /// @param start_pos - стартовый адрес.
        ///
        /// @return <  0 - ошибка.
        /// @return >= 0 - количество считанных байт.
        virtual int read( void *buf, u_int count, u_int start_pos ) = 0;

        /// @brief Запись массива байт.
        ///        
        /// @param buf       - адрес буфера, откуда будут записываться данные.
        /// @param count     - количество считываемых байт.
        /// @param start_pos - стартовый адрес.
        ///
        /// @return <  0 - ошибка.
        /// @return >= 0 - количество записанных байт.
        virtual int write( void *buf, u_int count, u_int start_pos ) = 0;

        /// @brief Получение размера памяти в байтах.
        ///        
        /// @return - размер памяти в байтах.
        virtual u_int get_size() const = 0;
    };
//-----------------------------------------------------------------------------
/// @brief Работа напрямую с энергонезависимой ОЗУ. Представляет абстракцию от 
/// аппаратной реализации памяти.
class NV_memory : public i_memory
    {
    //  [ 1 ]
    /// @var total_size
    /// @brief Общий размер памяти.
    //  [ 2 ]
    /// @var available_start_pos
    /// @brief Начальный доступный адрес.
    /// @details Для пропуска зарезервированной системной области.
    //  [ 3 ]
    /// @var available_end_pos
    /// @brief Конечный доступный адрес.
    /// @details Для пропуска зарезервированной системной области.
    u_int total_size;           // 1   
    u_int available_start_pos;  // 2   
    u_int available_end_pos;    // 3   

    public:
        /// @param total_size           - общий размер памяти.
        /// @param available_start_pos  - начальный доступный адрес.
        /// @param available_end_pos    - конечный доступный адрес.
        NV_memory( u_int total_size, u_int available_start_pos, 
            u_int available_end_pos );

        /// @brief Метод интерфейса @ref i_memory.
        u_int get_size() const
            {
            return total_size;
            }

        /// @brief Получение начального доступного для работы адреса памяти.
        ///        
        /// @return - начальный доступный для работы адрес памяти.
        u_int get_available_start_pos() const
            {
            return available_start_pos;
            }

        /// @brief Получение конечного доступного для работы адреса памяти.
        ///        
        /// @return - конечный доступный для работы адрес памяти.
        u_int get_available_end_pos() const
            {
            return available_end_pos;
            }
    };
//-----------------------------------------------------------------------------
/// @brief Работа с блоком памяти.
class memory_range: public i_memory
    {
    friend NV_memory_manager;

    public:
        /// @brief Метод интерфейса @ref i_memory.
        u_int get_size() const
            {
            return size;
            }

        /// @brief Метод интерфейса @ref i_memory.
        int read( void *buf, u_int count, u_int start_pos );

        /// @brief Метод интерфейса @ref i_memory.
        int write( void *buf, u_int count, u_int start_pos );

    private:
    //  [ 1 ]
    /// @var memory
    /// @brief Указатель на объект памяти.
    //  [ 2 ]
    /// @var start_pos
    /// @brief Начальный адрес.
    //  [ 3 ]
    /// @var size
    /// @brief Размер.
    i_memory    *memory;    // 1
    u_int       start_pos;  // 2
    u_int       size;       // 3

    /// @brief Закрытый конструктор.
    ///
    /// Создание объектов осуществляется через метод @ref get_memory_block 
    /// класса @ref NV_memory_manager.
    memory_range( i_memory *memory, u_int start_pos, u_int size );

    /// @brief Проверка на корректность параметров.
    /// 
    /// @return - результат проверки
    ///    0 - Ок.
    ///    1 - Ошибка.
    int check_params( u_int count, u_int start_pos );   
    };
//-----------------------------------------------------------------------------
/// @brief Работа с энергонезависимой ОЗУ. Представляет абстракцию от 
/// физического распределения памяти.
class NV_memory_manager
    {
    public:
        /// @brief Типы энергонезависимой памяти.
        enum MEMORY_TYPE
            {
            MT_NVRAM,
            MT_EEPROM,
            };
        
        /// @brief Получение объекта для работы с блоком памяти.
        ///        
        /// @param m_type - тип памяти, откуда будет выделятся память.
        /// @param count  - размер в байтах.
        ///
        /// @return =  0 - ошибка.
        /// @return >  0 - указатель на объект выделенного блока памяти.
        virtual memory_range* get_memory_block( MEMORY_TYPE m_type,
            u_int count );
                
    protected:
        NV_memory_manager();

        /// @brief Работа с энергонезависимой ОЗУ (non-volatile random access 
        /// memory).
        ///
        /// Данная память питается от батарейки, не имеет ограничений на  
        /// количество циклов записи/чтения.
        NV_memory *PAC_NVRAM;

        /// @brief Работа с энергонезависимой ОЗУ (Electrically Erasable 
        /// Programmable Read-Only Memory).
        ///
        /// Имеет ограничения на количество циклов записи/чтения - 1 миллион.
        NV_memory *PAC_EEPROM;

        u_int last_NVRAM_pos;  ///< Индекс крайнего свободного элемента NVRAM.
        u_int last_EEPROM_pos; ///< Индекс крайнего свободного элемента EEPROM.
    };
//-----------------------------------------------------------------------------
#endif // SYS_H