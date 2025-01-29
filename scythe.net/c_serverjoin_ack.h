#pragma once
#include "c_buffer.h"
#include "net_constants.h"



namespace scythe
{
	namespace net
	{
		class c_serverjoin_ack
		{
		public:
			inline c_serverjoin_ack(serverjoin_result p_res = serverjoin_result::none) : result(p_res) {}


			serverjoin_result result = serverjoin_result::none;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}