#pragma once
#include "c_buffer.h"
#include "net_constants.h"





namespace scythe
{
	namespace net
	{
		class c_channelmessage_ack
		{
		public:
			inline c_channelmessage_ack(uint64_t p_server_id = 0, uint64_t p_channel_id = 0, uint64_t p_sender_id = 0, uint64_t p_message_id = 0, std::string p_sender_nickname = "", std::string p_message = "") : server_id(p_server_id), channel_id(p_channel_id), sender_id(p_sender_id), message_id(p_message_id), sender_nickname(p_sender_nickname), message(p_message) {}


			uint64_t server_id = 0;
			uint64_t channel_id = 0;
			uint64_t sender_id = 0;
			uint64_t message_id = 0;
			std::string sender_nickname = "";
			std::string message = "";

			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}