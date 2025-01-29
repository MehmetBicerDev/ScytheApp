#include "c_tcpserver.h"
#include "c_server.h"
#include "c_voicechannel.h"
#include "c_voicechannel_join_ack.h"
#include "c_voicechannel_leave_ack.h"
void scythe::c_voicechannel::join(net::c_session* session)
{

	connected_sessions.push_back(session);
	broadcast_message((uint32_t)net::message_opcode::voicechannel_join_ack, net::c_voicechannel_join_ack(channel_id, session->account->account_id, session->account->nickname).write(), false);
}

void scythe::c_voicechannel::leave(net::c_session* session)
{
	connected_sessions.erase(std::remove(connected_sessions.begin(), connected_sessions.end(), session), connected_sessions.end());
	broadcast_message((uint32_t)net::message_opcode::voicechannel_leave_ack, net::c_voicechannel_leave_ack(channel_id, session->account->account_id).write(), false);

}



void scythe::c_voicechannel::broadcast_message(uint32_t id, net::c_buffer message, bool to_channel, net::c_session* self_session)
{

	if (to_channel)
	{

		for (auto session : connected_sessions)
		{
			if (session == self_session) continue;

			session->send_message(id, message);

		}
	}
	else
	{
		for (auto account : owner->accounts)
		{
			if (!account) continue;
			auto session = owner->base_server->get_session_by_account_id(account->account_id);

			if (session)
			{
				session->send_message(id, message);
			}
		}
	}
}

bool scythe::c_voicechannel::is_connected(uint64_t id)
{
	for (auto session : connected_sessions)
		if (session->account->account_id == id)
			return true;
	return false;
}

void scythe::c_voicechannel::client_leave(uint64_t account_id)
{
	for(auto session : connected_sessions)
		if (session->account->account_id == account_id)
		{
			connected_sessions.erase(std::remove(connected_sessions.begin(), connected_sessions.end(), session), connected_sessions.end());
			break;
		}
}

void scythe::c_voicechannel::client_join(c_account* account)
{
	net::c_session* session = new net::c_session();

	session->account = account;

	connected_sessions.push_back(session);
}