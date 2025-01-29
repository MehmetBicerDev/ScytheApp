#include "c_tcpserver.h"
#include "c_message.h"
scythe::net::c_tcpserver::c_tcpserver(int p_port, int p_max_clients) : port(p_port), max_clients(p_max_clients)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logger::error("could not initialize wsa.");
        return;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        logger::error("failed to create socket.");;
        WSACleanup();
        return;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt)) < 0) {
        logger::error("setsockopt failed.");;
        closesocket(server_fd);
        WSACleanup();
        return;
    }
    u_long mode = 1;  
    if (ioctlsocket(server_fd, FIONBIO, &mode) != 0) {
        logger::error("failed to set non-blocking mode for server socket");;
        closesocket(server_fd);
        WSACleanup();
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
        logger::error("bind failed");
        closesocket(server_fd);
        WSACleanup();
        return;
    }
    if (listen(server_fd, max_clients) < 0) {
        logger::error("listen failed");
        closesocket(server_fd);
        WSACleanup();
        return;
    }
}

scythe::net::c_tcpserver::~c_tcpserver()
{
    for (auto& [session_id, session] : clients) {
        delete session;
    }
    closesocket(server_fd);
    WSACleanup();
}

void scythe::net::c_tcpserver::start()
{
    fd_set readfds;
    SOCKET max_sd;

    logger::info("server listening on port {}", port);

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (auto& [session_id, session] : clients) {
            SOCKET client_fd = session->get_socket_fd();
            if (client_fd > 0) {
                FD_SET(client_fd, &readfds);
                if (client_fd > max_sd) max_sd = client_fd;
            }
        }

        int activity = select(0, &readfds, nullptr, nullptr, nullptr);
        if (activity == SOCKET_ERROR) {
            logger::error("select failed with error : {}", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_fd, &readfds)) {
            handle_new_connection();
        }

        for (auto client : clients) {
            if (FD_ISSET(client.second->get_socket_fd(), &readfds)) {
                handle_client_activity(client.second);
            }
        }
    }
}

scythe::net::c_session* scythe::net::c_tcpserver::get_session_by_id(int session_id)
{
    auto it = clients.find(session_id);
    if (it != clients.end()) {
        return it->second;
    }
    return nullptr;
}

scythe::net::c_session* scythe::net::c_tcpserver::get_session_by_account_id(uint64_t account_id)
{
    for (auto client : clients)
        if (client.second->account && client.second->account->account_id == account_id)
            return client.second;

    return nullptr;
}

void scythe::net::c_tcpserver::broadcast_message(uint32_t id, c_buffer buffer, c_session* self_session)
{
    for (auto client : clients)
    {
        if (client.second == self_session) continue;

        client.second->send_message(id, buffer);
    }
}

void scythe::net::c_tcpserver::handle_new_connection()
{
    SOCKET new_socket;
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);

    new_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_address), &addrlen);

    if (new_socket == INVALID_SOCKET) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            logger::error("Accept failed with error:{}", error);
        }
        return;
    }

    logger::info("new connection, socket fd: {}", new_socket);

    int session_id = ++connection_count;
    auto new_session = new c_session(new_socket, session_id);
    clients[connection_count] = new_session;
}

void scythe::net::c_tcpserver::handle_client_activity(c_session* session)
{
    if (!session->read_data()) {
        remove_client(session);
        return;
    }

    c_packet m_packet;

    if (m_packet.read(session->received_buffer))
    {
        handle_packet(session,m_packet.packet_type, m_packet.message_buffer);

        session->received_buffer.remove(m_packet.length + 7);
    }
}

void scythe::net::c_tcpserver::remove_client(c_session* session)
{
    on_client_disconnected(session);
    logger::info("removing client fd: {}", session->get_socket_fd());
    clients.erase(session->get_session_id());
    delete session;
}

void scythe::net::c_tcpserver::handle_packet(c_session* session, packet_type_opcode opcode, c_buffer buffer)
{
    switch (opcode)
    {
    case packet_type_opcode::message:
    {
        c_message message;
        if (message.read(buffer))
        {
            on_message_received(session, message.id, message.data);

        }
        break;
    }
    }
}

