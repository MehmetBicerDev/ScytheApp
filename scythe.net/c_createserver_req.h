#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_createserver_req
		{
		public:
			inline c_createserver_req(std::string m_name = "") : name(m_name) {}


			std::string name = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}