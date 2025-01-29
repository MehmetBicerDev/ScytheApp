#pragma once
#include <c_server.h>




namespace scythe
{
	namespace server_manager
	{
		uint64_t get_new_message_id();
		uint64_t get_date_time();

		c_server* create_server(std::string name, std::string invite_id);
		c_server* get_server_by_id(uint64_t id);
		c_server* get_server_by_invite_id(std::string invite_id);


		c_voicechannel* create_voicechannel(c_server* server, std::string name);
		c_voicechannel* get_voicechannel_by_id(uint64_t id);

		c_channel* create_channel(c_server* server, std::string name);
		c_channel* get_channel_by_id(uint64_t id);

	


		std::vector<c_server*> get_user_servers(uint64_t id);
	}
}