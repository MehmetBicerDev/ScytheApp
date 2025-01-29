#pragma once
#include "c_buffer.h"
#include "net_constants.h"



namespace scythe
{
	namespace net
	{
		class c_channelcreate_req
		{
		public:
			inline c_channelcreate_req(uint64_t sid = 0,channel_type t = channel_type::none, std::string m_name = "") : server_id(sid), type(t), name(m_name) {}


			uint64_t server_id = 0;
			channel_type type = channel_type::none;
			std::string name = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}