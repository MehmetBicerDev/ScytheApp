#include "server_manager.h"
#include "scythe_server.h"



namespace scythe
{
	namespace server_manager
	{
		std::mutex mtx;
		uint64_t new_server_id = 0;
		uint64_t new_voice_channel = 1;
		uint64_t new_message_id = 0;
		std::unordered_map<uint64_t, c_server*> servers;


		uint64_t get_new_message_id()
		{
			mtx.lock();
			++new_message_id;
			mtx.unlock();

			return new_message_id;
		}

		uint64_t get_date_time()
		{
			return GetTickCount64(); // Change this later.
		}

		c_server* create_server(std::string name, std::string invite_id)
		{
			mtx.lock();

			++new_server_id;
			c_server* gserver = new c_server();

			gserver->id = new_server_id;
			gserver->name = name;
			gserver->base_server = server;
			gserver->invite_id = invite_id;
			gserver->base_server = server;

			servers[new_server_id] = gserver;

			mtx.unlock();
			return gserver;
		}
		c_server* get_server_by_id(uint64_t id)
		{
			return servers.find(id) == servers.end() ? nullptr : servers.at(id);
		}
		c_server* get_server_by_invite_id(std::string invite_id)
		{
			for (auto server : servers)
				if (server.second->invite_id == invite_id) 
					return server.second;
			return nullptr;
		}
		c_voicechannel* create_voicechannel(c_server* server, std::string name)
		{
			mtx.lock();
			++new_voice_channel;
			mtx.unlock();
			return server->add_voicechannel(new_voice_channel, name);
		}
		c_voicechannel* get_voicechannel_by_id(uint64_t id)
		{
			for (auto server : servers)
				for (auto channel : server.second->voicechannels)
					if (channel.second->channel_id == id)
						return channel.second;
			return nullptr;
		}
		c_channel* create_channel(c_server* server, std::string name)
		{
			mtx.lock();
			++new_voice_channel;
			mtx.unlock();
			return server->add_channel(new_voice_channel, name);
		}
		c_channel* get_channel_by_id(uint64_t id)
		{
			for (auto server : servers)
				for (auto channel : server.second->messagechannels)
					if (channel.second->id == id)
						return channel.second;
			return nullptr;
		}
		std::vector<c_server*> get_user_servers(uint64_t id)
		{
			std::vector<c_server*> user_servers;
			for (auto server : servers)
				for (auto member : server.second->accounts)
					if (member->account_id == id)
						user_servers.push_back(server.second);
			return user_servers;
		}
	}
}