#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_voicechannel_join_ack
		{
		public:
			inline c_voicechannel_join_ack(uint32_t p_cid = 0, uint64_t p_uid = 0, std::string p_nickname = "") : channel_id(p_cid), user_id(p_uid), nickname(p_nickname) {}

			uint64_t channel_id = 0;
			uint64_t user_id = 0;
			std::string nickname = "";


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}