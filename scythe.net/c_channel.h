#pragma once
#include "c_channelmessage.h"
#include "c_session.h"



namespace scythe
{
	class c_server;

	class c_channel 
	{
	public:
		inline c_channel(){}

		std::mutex mtx;

		uint64_t id = 0;
		uint64_t server_id = 0;
		std::string name = "";

		std::vector<c_channelmessage*> messages;

		c_server* owner;


		void add_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message);

		void on_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message);

		void on_session_online(net::c_session* session);
	};
}