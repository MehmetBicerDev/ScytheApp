#include "c_server.h"
#include "c_tcpserver.h"
#include "net_constants.h"

#include "c_server_ack.h"
void scythe::c_server::on_session_online(net::c_session* session)
{
	session->send_message((uint32_t)net::message_opcode::server_ack, net::c_server_ack(this).write());

	for (auto channel : messagechannels)
	{
		channel.second->on_session_online(session);
	}

}
void scythe::c_server::on_session_offline(net::c_session* session)
{
}
void scythe::c_server::add_channel(c_channel* channel)
{
	messagechannels[channel->id] = channel;
}
scythe::c_channel* scythe::c_server::add_channel(uint64_t channel_id, std::string name)
{
	c_channel* channel = new c_channel();
	channel->id = channel_id;
	channel->server_id = id;
	channel->name = name;
	channel->owner = this;

	add_channel(channel);

	return channel;
}
void scythe::c_server::add_voicechannel(c_voicechannel* channel)
{
	voicechannels[channel->channel_id] = channel;
}

scythe::c_voicechannel* scythe::c_server::add_voicechannel(uint64_t channel_id, std::string channel_name)
{
	c_voicechannel* channel = new c_voicechannel();

	channel->channel_id = channel_id;
	channel->channel_name = channel_name;
	channel->server_id = id;
	channel->owner = this;

	add_voicechannel(channel);

	return channel;
}

void scythe::c_server::broadcast(uint32_t message_id, net::c_buffer buffer, c_account* self_account)
{
	for (auto account : accounts)
	{
		if (account == self_account) continue;

		auto session = base_server->get_session_by_account_id(account->account_id);

		if (session)
			session->send_message(message_id, buffer);
	}
		
}

void scythe::c_server::add_member(c_account* account)
{
	accounts.push_back(account);
}

scythe::c_account* scythe::c_server::get_account_by_id(uint64_t id)
{
	for (auto account : accounts)
		if (account->account_id == id)
			return account;
	return nullptr;
}

bool scythe::c_server::is_member(c_account account)
{
	return (get_account_by_id(account.account_id) ? true : false);
}

bool scythe::c_server::is_member(net::c_session* session)
{
	return is_member(*session->account);
}

bool scythe::c_server::is_message_channel(uint64_t id)
{
	return (messagechannels.find(id) == messagechannels.end() ? false : true);
}
