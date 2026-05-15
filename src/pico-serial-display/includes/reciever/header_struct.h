#ifndef HEADER_STRUCT_H
#define HEADER_STRUCT_H

#include <cstdint>

struct Header
{

protected:
	Header();

public:

	virtual bool is_all_data_recieved() = 0;
	virtual uint8_t get_length() = 0;
	virtual uint8_t get_sequence() = 0;
};

#endif