#pragma once
#include "c_accountstate.h"
#include "c_friendchannel.h"




namespace scythe
{
	class c_account
	{
	public:
		c_account(){}


		uint64_t account_id = 0;
		std::string nickname = "";

		std::string email = "";
		std::string password = "";


		c_accountstate state;


		std::unordered_map<uint64_t, c_friendchannel*> friend_channels;

		std::vector<c_account*> friends;


		void friend_request(std::string nickname);
	};
}