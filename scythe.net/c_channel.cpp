#include "c_channel.h"
#include "c_server.h"
#include "c_message.h"
#include "c_channelmessage_ack.h"
#include "c_channelbulkmessage_ack.h"
void scythe::c_channel::add_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message)
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

void scythe::c_channel::on_message(uint64_t message_id, uint64_t user_id, uint64_t date_time, std::string nickname, std::string message)
{
	add_message(message_id, user_id, date_time, nickname, message);
	owner->broadcast((uint32_t)net::message_opcode::channelmessage_ack, net::c_channelmessage_ack(server_id, id, user_id,message_id,  nickname, message).write());
}

void scythe::c_channel::on_session_online(net::c_session* session)
{
	uint16_t message_size = messages.size();
	if (message_size)
	{
		uint16_t packet_message_size = message_size > 1000 ? 1000 : message_size;

		printf("Packet Message Size %d \n", packet_message_size);
		net::c_channelbulkmessage_ack channelbulkmessage;

		channelbulkmessage.channel_id = id;
		channelbulkmessage.server_id = owner->id;
		mtx.lock();

		for (uint16_t i = message_size - packet_message_size; i < message_size; i++)
		{
			channelbulkmessage.messages.push_back(messages.at(i));
		}

		mtx.unlock();

		session->send_message((uint32_t)net::message_opcode::channelbulkmessage_ack, channelbulkmessage.write());
	}
}
