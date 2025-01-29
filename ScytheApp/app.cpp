#include "app.h"





namespace scythe
{
	namespace app
	{

		c_voicechat* voicechat = nullptr;
		scythe::net::scythe_client* client = nullptr;
		scythe::net::c_session* own_session = nullptr;
		scythe::c_windowframework* window_framework = nullptr;

		std::string auth_result_message = "...";
		bool login_success = false;

		std::vector<c_server_ui_item> server_items;

		std::unordered_map<uint64_t, c_server*> servers;

		net::client_state client_state = net::client_state::none;
		c_voicechannel* get_voicechannel_by_id(uint64_t m_id)
		{
			for (auto server : servers)
				for (auto vc : server.second->voicechannels)
					if (vc.second->channel_id == m_id)
						return vc.second;
			return nullptr;
		}

		c_channel* get_messagechannel_by_id(uint64_t m_sid, uint64_t m_id)
		{
			auto server = servers[m_sid];
			for (auto mc : server->messagechannels)
				if (mc.second->id == m_id)
					return mc.second;
			return nullptr;
		}

		c_channel* get_messagechannel_by_id(uint64_t m_id)
		{
			for (auto server : servers)
				for (auto mc : server.second->messagechannels)
					if (mc.second->id == m_id)
						return mc.second;
			return nullptr;
		}
		c_account* get_account_from_server_by_id(uint64_t m_id)
		{
			for (auto server : servers)
			{
				auto account = server.second->get_account_by_id(m_id);
				if (account) return account;
			}
			return nullptr;
		}
	}
}