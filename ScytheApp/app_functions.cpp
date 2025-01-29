#include "app_functions.h"
#include "app.h"

//packets
#include <c_channelmessage_req.h>
#include <c_channelcreate_req.h>
#include <c_voicechannel_join_req.h>
#include <c_createserver_req.h>
#include <c_serverjoin_req.h>
void scythe::app::send_chat_message(uint64_t server_id, uint64_t channel_id, std::string message)
{
	logger::info("sent chat_message (sid:{}, cid:{}, message:{})", server_id, channel_id, message);
	client->send_message((uint32_t)scythe::net::message_opcode::channelmessage_req, scythe::net::c_channelmessage_req(server_id, channel_id, message).write());
}

void scythe::app::send_create_channel(uint64_t server_id, net::channel_type channel_type, std::string name)
{
	logger::info("sent create_channel (sid:{}, ct:{}, name:{})", server_id, (uint8_t)channel_type, name);
	scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::channelcreate_req, scythe::net::c_channelcreate_req(server_id, channel_type, name).write());
}

void scythe::app::send_voicechannel_join(uint64_t channel_id)
{
	logger::info("sent voice_channel_join (cid:{})", channel_id);
	scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::voicechannel_join_req, scythe::net::c_voicechannel_join_req(channel_id).write());

}

void scythe::app::send_create_server(std::string server_name)
{
	logger::info("sent create_server (sn:{})", server_name);
	scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::createserver_req, scythe::net::c_createserver_req(server_name).write());

}

void scythe::app::send_server_join(std::string invite_id)
{
	logger::info("sent server_join (iid:{})", invite_id);
	scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::serverjoin_req, scythe::net::c_serverjoin_req(invite_id).write());

}
