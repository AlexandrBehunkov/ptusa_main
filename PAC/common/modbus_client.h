#ifndef modbus_client_h__
#define modbus_client_h__
#include "tcp_client.h"

class modbus_client 
	{
	protected:
		tcp_client* tcpclient;
	public:
		modbus_client(unsigned int id, char* ip, unsigned int port = 502, unsigned long exchangetimeout = 50);
		int get_id();
		//���������� ������� ��������� modbus
		int read_discrete_inputs(unsigned int start_address, unsigned int quantity);
		int read_coils(unsigned int start_address, unsigned int quantity);
		int read_holding_registers(unsigned int address, unsigned int quantity);
		int read_input_registers(unsigned int address, unsigned int quantity);
		int write_coil(unsigned int address, unsigned char value);
		int force_multiply_coils(unsigned int address, unsigned int quantity);
		int write_multiply_registers(unsigned int address, unsigned int quantity);
		//������� ��� ������ � ������� �� lua
		void zero_output_buff();
		void set_int2(unsigned int address, int_2 value);
		int_2 get_int2(unsigned int address);
		void set_int4(unsigned int address, int_4 value);
		int_4 get_int4(unsigned int address);
		void set_float(unsigned int address, float value);
		float get_float(unsigned int address);
		void set_bit(unsigned int address, int value);
		int get_bit(unsigned int address);
		~modbus_client();
	};
#endif // modbus_client_h__
