#pragma once
#include "voice_chat.h"
#include "scythe_client.h"
#include "c_windowframework.h"
#include "ui_items.h"
#include <c_server.h>

namespace scythe
{
	namespace app
	{
		extern scythe::net::c_session* own_session;
		extern c_voicechat* voicechat;
		extern scythe::net::scythe_client* client;
		extern scythe::c_windowframework* window_framework;


		extern std::string auth_result_message;
		extern bool login_success;


		extern net::client_state client_state;

		extern std::unordered_map<uint64_t, c_server*> servers;


		c_channel* get_messagechannel_by_id(uint64_t m_id);
		c_channel* get_messagechannel_by_id(uint64_t m_sid, uint64_t m_id);

		c_voicechannel* get_voicechannel_by_id(uint64_t m_id);

		c_account* get_account_from_server_by_id(uint64_t m_id);
	}
}