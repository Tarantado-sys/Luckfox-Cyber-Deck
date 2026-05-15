#ifndef PROTOCOL_HEADER_H
#define PROTOCOL_HEADER_H
#include "protocol_header_abstract.h"

struct Protocol_Header : public Header
{
	bool is_all_data_recieved() override;
	uint8_t get_length() const override;
	bool get_sequence() const override;
}

#endif 