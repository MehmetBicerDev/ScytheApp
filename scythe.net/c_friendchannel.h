#pragma once
#include "c_buffer.h"
#include "net_constants.h"
#include "c_channelmessage.h"


namespace scythe
{
	namespace net
	{
		class c_tcpserver;
	}
	class c_account;
	class c_friendchannel
	{
	public:
		inline c_friendchannel(){}


		uint64_t id = 0;
		bool is_group_chat = false;

	public:
		net::c_tcpserver* base_server = nullptr;
		std::mutex mtx;

	public:
		std::vector<c_channelmessage*> messages;
		std::vector<c_account*> members;
	public:
		void on_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message);
		void add_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message);

		std::string get_chat_name(c_account* owner);

		void broadcast(uint32_t message_id, net::c_buffer buffer, c_account* self_account = nullptr);

	};
}