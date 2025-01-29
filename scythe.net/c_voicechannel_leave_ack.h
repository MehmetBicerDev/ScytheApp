#pragma once
#include "c_buffer.h"




namespace scythe
{
	namespace net
	{
		class c_voicechannel_leave_ack
		{
		public:
			inline c_voicechannel_leave_ack(uint32_t p_cid = 0, uint64_t p_uid = 0) : channel_id(p_cid), user_id(p_uid) {}

			uint64_t channel_id = 0;
			uint64_t user_id = 0;


			bool read(c_buffer& buffer);

			c_buffer write();
		};
	}
}