#pragma once
#include "c_buffer.h"


namespace scythe
{
	namespace net
	{
		class c_friendrequest_req
		{
		public:
			inline c_friendrequest_req(std::string m_username = "") : username(m_username){}


			std::string username = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}