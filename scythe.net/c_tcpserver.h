#pragma once
#include "c_session.h"






namespace scythe
{
	namespace net
	{
		class c_tcpserver
		{
		public:
			c_tcpserver(int port, int max_clients);
			~c_tcpserver();

		public:
			void start();

			c_session* get_session_by_id(int session_id);
			c_session* get_session_by_account_id(uint64_t account_id);

			virtual void on_message_received(c_session* session, uint32_t id, c_buffer message) = 0;
			virtual void on_client_disconnected(c_session* session) = 0;

			void broadcast_message(uint32_t id, c_buffer buffer, c_session* self_session = nullptr);
		private:
			SOCKET server_fd = 0;
			int port = 0;
			int max_clients = 0;
			std::atomic<int> connection_count;

			std::unordered_map<int, c_session*> clients;  // Map session ID to session
			struct sockaddr_in address;

			void handle_new_connection();
			void handle_client_activity(c_session* session);
			void remove_client(c_session* session);
			void handle_packet(c_session* session, packet_type_opcode opcode, c_buffer buffer);


		};
	}
}