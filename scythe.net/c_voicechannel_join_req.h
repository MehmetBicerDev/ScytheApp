#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_voicechannel_join_req
		{
		public:
			inline c_voicechannel_join_req(uint64_t p_id = 0) : id(p_id) {}


			uint64_t id = 0;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}