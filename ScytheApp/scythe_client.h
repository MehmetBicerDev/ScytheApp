#pragma once
#include <c_tcpclient.h>






namespace scythe
{
	namespace net
	{
		class scythe_client : public c_tcpclient
		{
		public:
			scythe_client(const std::string& server_ip, int port) : c_tcpclient(server_ip, port){}

			// Inherited via c_tcpclient
			void on_message_received(uint32_t id, c_buffer message) override;
		};
	}
}
