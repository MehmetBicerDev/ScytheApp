#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_register_req
		{
		public:
			inline c_register_req(std::string p_nickname = "", std::string p_email = "", std::string p_password = "") :nickname(p_nickname), email(p_email), password(p_password) {}


			std::string nickname = "";
			std::string email = "";
			std::string password = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}