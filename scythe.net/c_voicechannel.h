#pragma once
#include "c_session.h"





namespace scythe
{
	class c_server;
	class c_voicechannel
	{
	public:
		c_voicechannel(){}


		std::uint64_t channel_id = 0;
		std::string channel_name = "";

		std::uint64_t server_id = 0;


		// online

		std::vector<net::c_session*> connected_sessions;


		c_server* owner = nullptr;

		void join(net::c_session* session);
		void leave(net::c_session* session);


		void broadcast_message(uint32_t id, net::c_buffer message, bool to_channel, net::c_session* self_session = nullptr);


		bool is_connected(uint64_t id);


		void client_leave(uint64_t account_id);

		void client_join(c_account* account);

	};
}