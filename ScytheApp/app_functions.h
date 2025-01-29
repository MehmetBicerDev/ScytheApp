#pragma once
#include <netincludes.h>
#include <net_constants.h>
namespace scythe
{
	namespace app
	{
		void send_chat_message(uint64_t server_id, uint64_t channel_id, std::string message);

		void send_create_channel(uint64_t server_id, net::channel_type channel_type, std::string name);

		void send_voicechannel_join(uint64_t channel_id);

		void send_create_server(std::string server_name);

		void send_server_join(std::string invite_id);
	}
}