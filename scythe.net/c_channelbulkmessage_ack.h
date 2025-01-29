#pragma once
#include "c_buffer.h"
#include "net_constants.h"
#include "c_channelmessage.h"




namespace scythe
{
	namespace net
	{
		class c_channelbulkmessage_ack
		{
		public:
			inline c_channelbulkmessage_ack(uint64_t p_server_id = 0, uint64_t p_channel_id = 0) : server_id(p_server_id), channel_id(p_channel_id) {}


			uint64_t server_id = 0;
			uint64_t channel_id = 0;

			std::vector<c_channelmessage*> messages;

			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}