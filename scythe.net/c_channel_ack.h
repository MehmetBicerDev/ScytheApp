#pragma once
#include "c_buffer.h"
#include "net_constants.h"





namespace scythe
{
	namespace net
	{
		class c_channel_ack
		{
		public:
			inline c_channel_ack(channel_type t = channel_type::none, std::string n= "", uint64_t s = 0, uint64_t vci = 0) : type(t), name(n), server_id(s), voice_channel_id(vci) {}


			channel_type type = channel_type::none;
			std::string name = "";
			uint64_t server_id = 0;
			uint64_t voice_channel_id = 0;

			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}