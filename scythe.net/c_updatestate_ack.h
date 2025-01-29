#pragma once
#include "c_buffer.h"
#include "net_constants.h"



namespace scythe
{
	namespace net
	{
		class c_updatestate_ack
		{
		public:
			inline c_updatestate_ack(client_state p_state = client_state::none) : state(p_state) {}


			client_state state = client_state::none;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}