#pragma once
#include "c_buffer.h"
#include "net_constants.h"




namespace scythe
{
	namespace net
	{
		class c_packet
		{
		public:
			inline c_packet(){}

			uint16_t packet_body_id = 0x8675;
			uint32_t length = 0;
			packet_type_opcode packet_type = packet_type_opcode::none;
			c_buffer message_buffer;


			bool read(c_buffer& buffer);
			c_buffer write();
		};
	}
}