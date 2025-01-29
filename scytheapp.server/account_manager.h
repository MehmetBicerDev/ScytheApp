#pragma once
#include <c_account.h>




namespace scythe
{
	namespace account_manager
	{
		c_account* create_account(std::string email, std::string password, std::string nickname);
		c_account* get_account(std::string email);
		bool is_valid_login(std::string email, std::string password);
		
		bool email_exists(std::string email);
		bool nickname_exists(std::string nickname);
	}
}