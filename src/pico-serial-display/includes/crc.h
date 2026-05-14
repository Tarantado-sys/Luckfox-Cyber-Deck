#ifndef CRC_H
#define CRC_H

#include <cstdint>

template<
	typename T,
	T Polynomial, 
	T Init_Value,
	T Final_Xor
>


class CRC
{
	static_assert(sizeof(T) >= 2);

	static T table[256];
	T current_crc;
	inline static bool table_initialized = false;

public:
	CRC() :
		current_crc(Init_Value)
	{
		if(table_initialized == false){
			init_table();
			table_initialized = true;
		}
	}

	static void init_table()
	{
		for (int i = 0; i < 256; i++)
		{
			T crc = static_cast<T>(i);
			for (int j = 0; j < 8; j++)
			{
				if (crc & 1){
					crc = (crc >> 1) ^ Polynomial; 
				}
				else {
					crc >>= 1;
				}
			}
			table[i] = crc;
		}
	}
	
	void update(unsigned char data)
	{
		current_crc = table[(current_crc ^ data) & 0xFF] ^ (current_crc >> 8);
	}

	T get_crc() const{
		return current_crc ^Final_Xor;
	}
};

using Protocol_CRC = 
	CRC< 
	uint16_t,
	0xA001,
	0xFFFF,
	0x0000
	>;

#endif