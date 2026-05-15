#ifndef PROTOCOL_HEADER_ABSTACT_H
#define PROTOCOL_HEADER_ABSTACT_H

#include <cstdint>

struct Header
{

protected:
	Header();

public:
	virtual bool is_all_data_recieved() = 0;
	virtual uint8_t get_length() const = 0;
	virtual bool get_sequence() const = 0;
};

#endif