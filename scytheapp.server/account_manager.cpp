#include "account_manager.h"


namespace scythe
{
	namespace account_manager
	{
		std::mutex mtx;
		uint64_t new_account_id = 1;

		std::unordered_map<uint64_t, c_account*> accounts;


		c_account* create_account(std::string email, std::string password, std::string nickname)
		{
			mtx.lock();

		
			c_account* account = new c_account();

			account->account_id = new_account_id;
			account->email = email;
			account->password = password;
			account->nickname = nickname;

			accounts[new_account_id] = account;
			++new_account_id;
			mtx.unlock();

			return account;
		}
		c_account* get_account(std::string email)
		{
			for (auto account : accounts)
				if (account.second->email == email)
					return account.second;
			return nullptr;
		}
		bool is_valid_login(std::string email, std::string password)
		{
			for (auto account : accounts)
				if (account.second->email == email && account.second->password == password)
					return true;
			return false;
		}
		bool email_exists(std::string email)
		{
			for (auto account : accounts)
				if (account.second->email == email)
					return true;
			return false;
		}
		bool nickname_exists(std::string nickname)
		{
			for (auto account : accounts)
				if (account.second->nickname == nickname)
					return true;
			return false;
		}
	}
}