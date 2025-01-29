#pragma once
#include "c_buffer.h"
#include "net_constants.h"



namespace scythe
{
	namespace net
	{
		class c_server_userjoin_info_ack
		{
		public:
			inline c_server_userjoin_info_ack(uint64_t sid =0 ,uint64_t p_account_id = 0, std::string p_nickname = "") : server_id(sid), account_id(p_account_id), nickname(p_nickname) {}

			uint64_t server_id = 0;
			uint64_t account_id = 0;
			std::string nickname = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}