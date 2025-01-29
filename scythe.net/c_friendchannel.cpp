#include "c_friendchannel.h"
#include "c_tcpserver.h"
#include "c_channelmessage_ack.h"

void scythe::c_friendchannel::on_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message)
{
	add_message(message_id, user_id, date_time, nickname, message);
	broadcast((uint32_t)net::message_opcode::channelmessage_ack, net::c_channelmessage_ack(id, id, user_id, message_id, nickname, message).write());
}

void scythe::c_friendchannel::add_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message)
{
	mtx.lock();
	c_channelmessage* cm = new c_channelmessage();

	cm->message = message;
	cm->channel_id = id;
	cm->date_time = date_time;
	cm->message_id = message_id;
	cm->nickname = nickname;
	cm->user_id = user_id;

	messages.push_back(cm);
	mtx.unlock();
}

std::string scythe::c_friendchannel::get_chat_name(c_account* owner)
{
	std::string channel_name = "";
	if (is_group_chat)
	{
		channel_name = "You, ";
		for (auto member : members)
			channel_name += member->nickname + ", ";
	}
	else
	{
		for (auto member : members)
			if (member != owner)
				channel_name = member->nickname;
	}
	return channel_name;
}

void scythe::c_friendchannel::broadcast(uint32_t message_id, net::c_buffer buffer, c_account* self_account)
{
	for (auto account : members)
	{
		if (account == self_account) continue;

		auto session = base_server->get_session_by_account_id(account->account_id);

		if (session)
			session->send_message(message_id, buffer);
	}
}
