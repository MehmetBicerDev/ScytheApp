#pragma once
#include "c_session.h"






namespace scythe
{
	namespace net
	{
		class c_tcpclient
		{
		public:
			c_tcpclient(const std::string& server_ip, int port);
			~c_tcpclient();

			void handle_packet(packet_type_opcode opcode, c_buffer buffer);

			bool connect_to_server();
			bool send_data(const BYTE* data, size_t length);
			void receive_data();
			void handle_client_activity();


			virtual void on_message_received(uint32_t id, c_buffer message) = 0;



			void send_message(uint32_t id, c_buffer message);
		private:
			c_session* base_session = nullptr;
			SOCKET client_socket;
			std::string server_ip;
			int port;
			struct sockaddr_in server_address;

			c_buffer received_buffer;

			std::thread m_activity_thread;
			std::thread m_receivedata_thread;
		};
	}
}