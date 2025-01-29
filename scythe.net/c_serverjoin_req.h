#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_serverjoin_req
		{
		public:
			inline c_serverjoin_req(std::string p_invite = "") : invite_id(p_invite) {}


			std::string invite_id = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}