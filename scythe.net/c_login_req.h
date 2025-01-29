#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_login_req
		{
		public:
			inline c_login_req(std::string p_username = "", std::string p_password = "") : username(p_username), password(p_password) {}



			std::string username = "";
			std::string password = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}