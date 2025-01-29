#include "scythe_client.h"
#include "app.h"
#include "voice_chat.h"

#include <c_updatestate_ack.h>
#include <c_voice_data_ack.h>
#include <c_voicechannel_join_ack.h>
#include <c_voicechannel_leave_ack.h>
#include <c_auth_ack.h>
#include <c_serverjoin_ack.h>
#include <c_server_ack.h>
#include <c_channel_ack.h>
#include <c_server_userjoin_info_ack.h>

#include <c_channelmessage.h>
#include <c_channelmessage_ack.h>
#include <c_channelbulkmessage_ack.h>
void scythe::net::scythe_client::on_message_received(uint32_t id, c_buffer message)
{
	logger::info("Received Message ID={}", id);
	switch ((message_opcode)id)
	{
	case message_opcode::updatestate_ack:
	{
		c_updatestate_ack updatestate_ack;
		if (updatestate_ack.read(message))
		{
			app::client_state = updatestate_ack.state;
		}
		break;
	}
	case message_opcode::voice_data_ack:
	{

		c_voice_data_ack voice_data_ack;
		if (voice_data_ack.read(message))
		{
			logger::info("received voice data");
			c_voicechat::voice_data.push_back(voice_data_ack);
		}
		break;
	}
	case message_opcode::auth_ack:
	{
		c_auth_ack auth_ack;
		if (auth_ack.read(message))
		{
			if (auth_ack.result == auth_result::success)
			{
				logger::info("auth_ack success(uid:{}, nick:{})", auth_ack.account_id, auth_ack.nickname);
				app::own_session->account->account_id = auth_ack.account_id;
				app::own_session->account->nickname = auth_ack.nickname;
				app::login_success = true;
				app::auth_result_message = "Success..";
			}
			else if (auth_ack.result == auth_result::invalid_id_or_password)
			{
				app::auth_result_message = "Invalid ID or Password.";
			}
			else if (auth_ack.result == auth_result::email_already_exists)
			{
				app::auth_result_message = "Email Already Exists.";
			}
			else if (auth_ack.result == auth_result::username_already_exists)
			{
				app::auth_result_message = "Username already exists.";
			}
			else if (auth_ack.result == auth_result::account_blocked)
			{
				app::auth_result_message = "Your account has been blocked.";
			}
			logger::info("Auth Result={}", (uint8_t)auth_ack.result);
		}
		break;
	}
	case message_opcode::voicechannel_join_ack:
	{
		c_voicechannel_join_ack channel_join_ack;
		logger::info("channel join ack");
		if (channel_join_ack.read(message))
		{
			auto vc = app::get_voicechannel_by_id(channel_join_ack.channel_id);
			if (vc) {
				if (channel_join_ack.user_id == app::own_session->account->account_id) {
					app::own_session->account->state.joined_voicechannel_id = channel_join_ack.channel_id;
					vc->connected_sessions.push_back(app::own_session);
				}
				else
				{
					vc->client_join(app::get_account_from_server_by_id(channel_join_ack.user_id));
				}
			}
			logger::info("User Joined={}({})", channel_join_ack.nickname, channel_join_ack.user_id);
		}
		break;
	}
	case message_opcode::voicechannel_leave_ack:
	{
		c_voicechannel_leave_ack voicechannel_leave_ack;
		if (voicechannel_leave_ack.read(message))
		{
			auto vc = app::get_voicechannel_by_id(voicechannel_leave_ack.channel_id);
			if (vc)
			{
				if (voicechannel_leave_ack.user_id == app::own_session->account->account_id)
				{
					app::own_session->account->state.joined_voicechannel_id = 0;
				}

				vc->client_leave(voicechannel_leave_ack.user_id);

			}
		}
		break;
	}
	case message_opcode::serverjoin_ack:
	{
		c_serverjoin_ack serverjoin_ack;
		if (serverjoin_ack.read(message))
		{
			logger::info("ServerJoin Result={}", (uint32_t)serverjoin_ack.result);
		}
		break;
	}
	case message_opcode::server_ack:
	{
		c_server_ack server_ack;
		if (server_ack.read(message))
		{
			logger::info("Received Server {}", server_ack.server->name);
			app::servers[server_ack.server->id] = server_ack.server;
		}
		else logger::info("failed to read server_ack.");
		break;
	}
	case message_opcode::channel_ack:
	{
		c_channel_ack channel_ack;
		if (channel_ack.read(message))
		{
			if (channel_ack.type == channel_type::voice)
			{
				auto voicechannel = new c_voicechannel();
				voicechannel->channel_id = channel_ack.voice_channel_id;
				voicechannel->channel_name = channel_ack.name;
				voicechannel->server_id = channel_ack.server_id;
				voicechannel->owner = app::servers[channel_ack.server_id];

				app::servers[channel_ack.server_id]->voicechannels[voicechannel->channel_id] = voicechannel;
			} 
			else
			{
				auto channel = new c_channel();

				channel->id = channel_ack.voice_channel_id;
				channel->name = channel_ack.name;
				channel->server_id = channel_ack.server_id;
				channel->owner = app::servers[channel_ack.server_id];

				
				app::servers[channel_ack.server_id]->messagechannels[channel->id] = channel;

			}
		}
		break;
	}
	case message_opcode::server_userjoin_info_ack:
	{
		c_server_userjoin_info_ack server_userjoin_info_ack;

		if (server_userjoin_info_ack.read(message))
		{
			auto server = app::servers[server_userjoin_info_ack.server_id];

			c_account* account = new c_account();

			account->account_id = server_userjoin_info_ack.account_id;
			account->nickname = server_userjoin_info_ack.nickname;

			server->accounts.push_back(account);
		}
		break;
	}
	case message_opcode::channelmessage_ack:
	{
		c_channelmessage_ack channelmessage_ack;
		if (channelmessage_ack.read(message))
		{
			auto server = app::servers.at(channelmessage_ack.server_id);
			auto channel = server->messagechannels.at(channelmessage_ack.channel_id);
			logger::info("received message(U:{}, S:{}, C:{}, M:{})",channelmessage_ack.sender_nickname, server->name, channel->name, channelmessage_ack.message);

			channel->add_message(channelmessage_ack.message_id, channelmessage_ack.sender_id, 0, channelmessage_ack.sender_nickname, channelmessage_ack.message);
		}
		break;
	}
	case message_opcode::channelbulkmessage_ack:
	{
		c_channelbulkmessage_ack channelbulkmessage_ack;
		if (channelbulkmessage_ack.read(message))
		{
			auto channel = app::get_messagechannel_by_id(channelbulkmessage_ack.server_id, channelbulkmessage_ack.channel_id);

			if (channel)
			{
				logger::info("Received {} bulk messages.", channelbulkmessage_ack.messages.size());
				channel->messages.insert(channel->messages.begin(), channelbulkmessage_ack.messages.begin(), channelbulkmessage_ack.messages.end());
			}
			else logger::error("could not find server or channel (sid:{}, cid:{})", channelbulkmessage_ack.server_id, channelbulkmessage_ack.channel_id);
		}
		break;
	}
	}
}
