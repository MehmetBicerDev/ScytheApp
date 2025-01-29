#include "c_tcpclient.h"
#include "c_message.h"



scythe::net::c_tcpclient::c_tcpclient(const std::string& server_ip, int port) : server_ip(server_ip), port(port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logger::error("failed to initialize wsa.");
        return;
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        logger::error("socket creation failed");
        WSACleanup();
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

   
}

scythe::net::c_tcpclient::~c_tcpclient()
{
    closesocket(client_socket);
    WSACleanup();
    if(m_activity_thread.joinable())
        m_activity_thread.detach();
    if (m_receivedata_thread.joinable())
        m_receivedata_thread.detach();
}

void scythe::net::c_tcpclient::handle_packet(packet_type_opcode opcode, c_buffer buffer)
{
    switch (opcode)
    {
    case packet_type_opcode::message:
    {
        c_message message;
        if (message.read(buffer))
        {
            on_message_received(message.id, message.data);
        }
        break;
    }
    }
}

bool scythe::net::c_tcpclient::connect_to_server()
{
    if (connect(client_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) == SOCKET_ERROR) {
        logger::error("connection to server failed");

        closesocket(client_socket);
        WSACleanup();
        return false;
    }

    logger::info("connection to server");

    m_activity_thread = std::thread(&scythe::net::c_tcpclient::handle_client_activity, this);
    m_receivedata_thread = std::thread(&scythe::net::c_tcpclient::receive_data, this);
    return true;
}

bool scythe::net::c_tcpclient::send_data(const BYTE* data, size_t length)
{
    if (send(client_socket, reinterpret_cast<const char*>(data), length, 0) == SOCKET_ERROR) {
        logger::error("send failed");

        return false;
    }
    return true;
}

void scythe::net::c_tcpclient::receive_data()
{
    std::vector<BYTE> buffer(1024 * 1024); 
    while (true) {
        ZeroMemory(buffer.data(), buffer.size());
        int valread = recv(client_socket, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

        if (valread == SOCKET_ERROR) {
            logger::error("receive failed");

            break;
        }
        else if (valread == 0) {
            logger::error("server closed the connection");

            break;
        }
        else {
            received_buffer.write_bytes(buffer.data(), valread);
        }
    }
}

void scythe::net::c_tcpclient::handle_client_activity()
{
    while (true){
        c_packet m_packet;
        if (m_packet.read(received_buffer))
        {
            handle_packet(m_packet.packet_type, m_packet.message_buffer);
        }
    }
}

void scythe::net::c_tcpclient::send_message(uint32_t id, c_buffer message)
{
    c_packet vpacket;
    c_message vmessage;

    vmessage.id = id;
    vmessage.data = message;

    auto data = vmessage.write();

    vpacket.length = data.size();
    vpacket.message_buffer = data;
    vpacket.packet_type = packet_type_opcode::message;

    auto packet_data = vpacket.write();
    send_data(packet_data.buffer(), packet_data.size());

}
