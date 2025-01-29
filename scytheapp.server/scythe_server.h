#pragma once
#include <c_tcpserver.h>



namespace scythe
{
	namespace net
	{
		class scythe_server : public c_tcpserver
		{
		public:
			scythe_server(int port, int max_clients) : c_tcpserver(port, max_clients){}


			// Inherited via c_tcpserver
			void on_message_received(c_session*, uint32_t id, c_buffer message) override;


			// Inherited via c_tcpserver
			void on_client_disconnected(c_session* session) override;


			void on_client_auth_success(c_session* session);
		};


	}
}

extern scythe::net::scythe_server* server;