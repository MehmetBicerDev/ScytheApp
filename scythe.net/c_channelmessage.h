#pragma once
#include "netincludes.h"







namespace scythe
{
	class c_channelmessage
	{
	public:
		inline c_channelmessage(){}

		uint64_t channel_id = 0;
		uint64_t message_id = 0;
		uint64_t user_id = 0;
		uint64_t date_time = 0;

		std::string nickname = "";
		std::string message = "";
	};
}