#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_voice_data_ack
		{
		public:
			inline c_voice_data_ack(uint32_t p_id = 0, c_buffer p_data = c_buffer()) : id(p_id), data(p_data) {}


			uint64_t id = 0;
			c_buffer data;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}