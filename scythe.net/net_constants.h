#pragma once
#include <iostream>




namespace scythe
{
	namespace net
	{
		enum class packet_type_opcode : uint8_t
		{
			none,

			//common
			message
		};


		enum class message_opcode : uint32_t
		{
			none,

			voice_data_req = 0x7FF1,
			login_req,
			register_req,
			voicechannel_join_req,
			voicechannel_leave_req,
			serverjoin_req,
			createserver_req,
			channelcreate_req,
			channelmessage_req,
			friendrequest_req,
			removefriend_req,

			voice_data_ack = 0x17F1,
			auth_ack,
			voicechannel_join_ack,
			voicechannel_leave_ack,
			serverjoin_ack,
			server_ack,
			channel_ack,
			server_userjoin_info_ack,
			channelmessage_ack,
			channelbulkmessage_ack,
			updatestate_ack,
			
		};


		enum class auth_result : uint8_t
		{
			none,
			success,
			invalid_id_or_password,
			email_already_exists,
			username_already_exists,
			account_blocked
		};

		enum class serverjoin_result : uint8_t
		{
			none,
			success,
			invalid_invite_id,
		};


		enum class channel_type : uint8_t
		{
			none,
			voice,
			chat
		};

		enum class client_state : uint8_t
		{
			none,
			waiting_for_auth,
			logged_in,
			waiting_for_serverinfo,
			client_loaded
		};

		enum class permission : uint8_t
		{
			read_messages = 0,
			send_messages = 1,
			create_invite = 2,
			kick_member = 3,
			ban_member = 4,
			manage_channels = 5,
			manage_roles = 6,
			view_audit_log = 7,
			join_voice_channels = 8,
			mute_members = 9,
			deafen_members = 10,
			max
		};
	}
}