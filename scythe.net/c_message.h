#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_message
		{
		public:
			inline c_message(uint32_t p_id = 0, c_buffer p_data = c_buffer()) : id(p_id), data(p_data){}


			uint32_t id = 0;
			c_buffer data;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}