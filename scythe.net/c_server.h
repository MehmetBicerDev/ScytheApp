#pragma once
#include "c_voicechannel.h"
#include "c_channel.h"
#include "c_account.h"




namespace scythe
{
	namespace net
	{
		class c_tcpserver;
	}
	class c_server
	{
	public:
		c_server(){}

		uint64_t id = 0;
		std::string name = "";

		std::string invite_id = "";

		std::unordered_map<uint64_t, c_voicechannel*> voicechannels;
		std::unordered_map<uint64_t, c_channel*> messagechannels;

		std::vector<c_account*> accounts;

		net::c_tcpserver* base_server;


		void on_session_online(net::c_session* session);
		void on_session_offline(net::c_session* session);

		void add_channel(c_channel* channel);
		c_channel* add_channel(uint64_t channel_id, std::string name);

		void add_voicechannel(c_voicechannel* channel);
		c_voicechannel* add_voicechannel(uint64_t channel_id, std::string channel_name);

		void broadcast(uint32_t message_id, net::c_buffer buffer, c_account* self_account = nullptr);

		void add_member(c_account* account);

		c_account* get_account_by_id(uint64_t id);

		bool is_member(c_account account);
		bool is_member(net::c_session* session);

		bool is_message_channel(uint64_t id);
	};
}