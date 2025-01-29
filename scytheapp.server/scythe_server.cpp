#include "scythe_server.h"
#include "account_manager.h"
#include "server_manager.h"

#include <c_voice_data_req.h>
#include <c_voice_data_ack.h>
#include <c_voicechannel_join_req.h>
#include <c_voicechannel_join_ack.h>

#include <c_login_req.h>
#include <c_register_req.h>
#include <c_auth_ack.h>

#include <c_serverjoin_req.h>
#include <c_serverjoin_ack.h>
#include <c_createserver_req.h>
#include <c_server_ack.h>
#include <c_channel_ack.h>
#include <c_channelcreate_req.h>
#include <c_server_userjoin_info_ack.h>

#include <c_channelmessage_req.h>
#include <c_channelmessage_ack.h>

#include <rand_util.h>
void scythe::net::scythe_server::on_message_received(c_session* session, uint32_t id, c_buffer message)
{
	//logger::info("message received {}", id);
	switch ((message_opcode)id)
	{
	case message_opcode::login_req:
	{
		c_login_req login_req;
		if (login_req.read(message))
		{
			if (session->account) return;
			c_auth_ack auth_ack;
			if (account_manager::is_valid_login(login_req.username, login_req.password))
			{
				auth_ack.result = auth_result::success;
				session->account = account_manager::get_account(login_req.username);

				auth_ack.account_id = session->account->account_id;
				auth_ack.nickname = session->account->nickname;

				logger::info("Login Success Email:{}, Password:{}, Nickname:{}, AccountID:{}", login_req.username, login_req.password, session->account->nickname, session->account->account_id);

				session->send_message((uint32_t)message_opcode::auth_ack, auth_ack.write());

				on_client_auth_success(session);
				return;
			}
			else
				logger::error("Invalid Login Email:{}, Password:{}", login_req.username, login_req.password);
		}
		break;
	}
	case message_opcode::register_req:
	{
		c_register_req register_req;
		if (register_req.read(message))
		{
			if (session->account) return;

			c_auth_ack auth_ack;
			if (account_manager::email_exists(register_req.email))
			{
				auth_ack.result = auth_result::email_already_exists;
				session->send_message((uint32_t)message_opcode::auth_ack, auth_ack.write());
				return;
			}

			if (account_manager::nickname_exists(register_req.nickname))
			{
				auth_ack.result = auth_result::username_already_exists;
				session->send_message((uint32_t)message_opcode::auth_ack, auth_ack.write());
				return;
			}


			logger::info("Account Created=({}, {}, {})", register_req.email, register_req.password, register_req.nickname);

			session->account = account_manager::create_account(register_req.email, register_req.password, register_req.nickname);

			auth_ack.account_id = session->account->account_id;
			auth_ack.nickname = register_req.nickname;
			auth_ack.result = auth_result::success;
			session->send_message((uint32_t)message_opcode::auth_ack, auth_ack.write());

			session->update_state(client_state::client_loaded);

		}
		break;
	}
	case message_opcode::voice_data_req:
	{
		if (!session->account || !session->account->state.joined_voicechannel_id) return;
		c_voice_data_req voice_data_req;

		if (voice_data_req.read(message))
		{
			auto channel = server_manager::get_voicechannel_by_id(session->account->state.joined_voicechannel_id);
			if (!channel) return;


			channel->broadcast_message((uint32_t)message_opcode::voice_data_ack, c_voice_data_ack(session->account->account_id, voice_data_req.data).write(), true, session);
		}
		break;
	}
	case message_opcode::voicechannel_join_req:
	{
		if (!session->account) return;

		c_voicechannel_join_req voicechannel_join_req;

		if (voicechannel_join_req.read(message))
		{
			
			if (session->account->state.joined_voicechannel_id)
			{
				auto channel = server_manager::get_voicechannel_by_id(session->account->state.joined_voicechannel_id);
				if (channel)
				{
					channel->leave(session);

					session->account->state.joined_voicechannel_id = 0;
				}
			}

			auto channel = server_manager::get_voicechannel_by_id(voicechannel_join_req.id);

			if (channel && !channel->is_connected(session->account->account_id))
			{
				session->account->state.joined_voicechannel_id = channel->channel_id;
				channel->join(session);
				logger::info("User Joined {}", session->account->nickname);
			}
			else logger::error("channel does not exist.");
		}
		break;
	}
	case message_opcode::voicechannel_leave_req:
	{
		if (!session->account) return;

		if (session->account->state.joined_voicechannel_id)
		{
			auto channel = server_manager::get_voicechannel_by_id(session->account->state.joined_voicechannel_id);
			if (channel)
			{
				channel->leave(session);
			}
		}

		break;
	}
	case message_opcode::serverjoin_req:
	{
		c_serverjoin_req serverjoin_req;
		c_serverjoin_ack serverjoin_ack;
	
		if (serverjoin_req.read(message))
		{
			auto server = server_manager::get_server_by_invite_id(serverjoin_req.invite_id);

			if (!server)
			{
				logger::warning("Invalid Invite ID!");
				serverjoin_ack.result = serverjoin_result::invalid_invite_id;
				session->send_message((uint32_t)message_opcode::serverjoin_ack, serverjoin_ack.write());

				return;
			}

			logger::info("joined to server({})", server->name);
			serverjoin_ack.result = serverjoin_result::success;
			session->send_message((uint32_t)message_opcode::serverjoin_ack, serverjoin_ack.write());
			//session->send_message((uint32_t)message_opcode::server_ack, c_server_ack(server).write());

			server->on_session_online(session);

			server->add_member(session->account);

			server->broadcast((uint32_t)message_opcode::server_userjoin_info_ack, c_server_userjoin_info_ack(server->id, session->account->account_id, session->account->nickname).write());
		}
		break;
	}
	case message_opcode::createserver_req:
	{
		c_createserver_req createserver_req;

		if (createserver_req.read(message))
		{
			logger::info("Server Created={}", createserver_req.name);
			auto server = server_manager::create_server(createserver_req.name, random_generator::generate_random_string(8));
			server->accounts.push_back(session->account);
			session->send_message((uint32_t)message_opcode::server_ack, c_server_ack(server).write());

		}
		break;
	}
	case message_opcode::channelcreate_req:
	{
		c_channelcreate_req channelcreate_req;

		if (channelcreate_req.read(message))
		{
			auto servers = server_manager::get_user_servers(session->account->account_id);
			logger::info("User Server Count {}", servers.size());
			for (auto server : servers)
			{
				if (server->id == channelcreate_req.server_id)
				{
					logger::info("Created Voice Channel");

					if (channelcreate_req.type == net::channel_type::voice)
					{
						auto channel = server_manager::create_voicechannel(server, channelcreate_req.name);

						server->broadcast((uint32_t)message_opcode::channel_ack, c_channel_ack(channelcreate_req.type, channelcreate_req.name, channel->server_id, channel->channel_id).write());
					}
					else
					{
						auto channel = server_manager::create_channel(server, channelcreate_req.name);

						server->broadcast((uint32_t)message_opcode::channel_ack, c_channel_ack(channelcreate_req.type, channelcreate_req.name, channel->server_id, channel->id).write());
					}
				}
				
			}
		}
		break;
	}
	case message_opcode::channelmessage_req:
	{
		c_channelmessage_req channelmessage_req;
		if (channelmessage_req.read(message))
		{
			auto server = server_manager::get_server_by_id(channelmessage_req.server_id);
			if (server && server->is_member(session) && server->is_message_channel(channelmessage_req.channel_id))
			{
				auto channel = server->messagechannels.at(channelmessage_req.channel_id);

				channel->on_message(server_manager::get_new_message_id(), session->account->account_id, server_manager::get_date_time(), session->account->nickname, channelmessage_req.message);
			}
		}
		break;
	}
	}
}

void scythe::net::scythe_server::on_client_disconnected(c_session* session)
{
	if (session->account)
	{
		if (session->account->state.joined_voicechannel_id)
		{
			auto channel = server_manager::get_voicechannel_by_id(session->account->state.joined_voicechannel_id);

			if (channel)
			{
				channel->leave(session);
			}
		}
	}
}

void scythe::net::scythe_server::on_client_auth_success(c_session* session)
{
	session->update_state(client_state::waiting_for_serverinfo);
	auto servers = server_manager::get_user_servers(session->account->account_id);

	for (auto server : servers)
	{
		server->on_session_online(session);


	}
	session->update_state(client_state::client_loaded);

}
