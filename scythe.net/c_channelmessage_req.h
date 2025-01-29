#pragma once
#include "c_buffer.h"
#include "net_constants.h"



namespace scythe
{
	namespace net
	{
		class c_channelmessage_req
		{
		public:
			inline c_channelmessage_req(uint64_t p_server_id = 0, uint64_t p_channel_id = 0 ,std::string p_message = "") :server_id(p_server_id), channel_id(p_channel_id), message(p_message) {}

			uint64_t server_id = 0;
			uint64_t channel_id = 0;
			std::string message = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}