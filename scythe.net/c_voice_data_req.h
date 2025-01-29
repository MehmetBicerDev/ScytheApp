#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_voice_data_req
		{
		public:
			inline c_voice_data_req(c_buffer p_data = c_buffer()) : data(p_data) {}



			c_buffer data;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}