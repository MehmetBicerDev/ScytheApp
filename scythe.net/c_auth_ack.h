#pragma once
#include "c_buffer.h"
#include "net_constants.h"



namespace scythe
{
	namespace net
	{
		class c_auth_ack
		{
		public:
			inline c_auth_ack(uint64_t uid = 0, std::string n = "", auth_result p_res = auth_result::none) :account_id(uid), nickname(n), result(p_res) {}

			uint64_t account_id = 0;
			std::string nickname = "";
			auth_result result = auth_result::none;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}