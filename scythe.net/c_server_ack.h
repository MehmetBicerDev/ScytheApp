#pragma once
#include "c_server.h"






namespace scythe
{
	namespace net
	{
		class c_server_ack
		{
		public:
			c_server_ack(c_server* p_server = 0) : server(p_server) {}


			c_server* server = nullptr;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}