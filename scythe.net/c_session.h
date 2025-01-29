#pragma once
#include "c_packet.h"
#include "c_account.h"
#include "net_constants.h"

namespace scythe
{
	namespace net
	{
		class c_session
		{
		public:
			c_session(int socket_fd = 0, int session_id = 0);
			~c_session();

		public:
			int get_socket_fd() const;
			int get_session_id() const;

			c_account* account = nullptr;

		public:
			bool read_data();
			void send_message(uint32_t id, c_buffer message);
			bool send_data(const BYTE* data, size_t length);

			c_buffer received_buffer;


			void update_state(client_state state);

		private:
			bool is_connected = false;
			int socket_fd = 0;
			int session_id = 0;
		};
	}
}