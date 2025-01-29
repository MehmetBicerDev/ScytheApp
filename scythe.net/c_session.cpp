#include "c_session.h"
#include "c_message.h"

#include "c_updatestate_ack.h"
scythe::net::c_session::c_session(int p_socket_fd, int p_session_id) : socket_fd(p_socket_fd), is_connected(true), session_id(p_session_id)
{
    u_long mode = 1;  // 1 to enable non-blocking mode
    if (ioctlsocket(socket_fd, FIONBIO, &mode) != 0) {
        logger::warning("failed to set non-blocking mode for socket {}", socket_fd);
        closesocket(socket_fd);
        is_connected = false;
    }
}

scythe::net::c_session::~c_session()
{
    if (is_connected) {
        closesocket(socket_fd);
    }
}

int scythe::net::c_session::get_socket_fd() const
{
    return socket_fd;
}

int scythe::net::c_session::get_session_id() const
{
    return session_id;
}

bool scythe::net::c_session::read_data()
{

    // Use dynamic allocation (heap) instead of stack allocation for large buffers
    std::vector<BYTE> buffer(1024 * 1024);  // Allocates 1MB on the heap


    // ZeroMemory or simply fill with zeros using vector's `assign` or `resize`
    ZeroMemory(buffer.data(), buffer.size());

    // Read data into the buffer
    int valread = recv(socket_fd, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

    if (valread == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            logger::warning("client read error, fd {}, error={}", socket_fd, error);
            closesocket(socket_fd);
            is_connected = false;
            return false;
        }
    }
    else if (valread == 0) {
        logger::info("client disconnected, fd:{}", socket_fd);
        closesocket(socket_fd);
        is_connected = false;
        return false;
    }
    else {
        // Write received data to the session's buffer
        received_buffer.write_bytes(buffer.data(), valread);
    }

    return true;
}

void scythe::net::c_session::send_message(uint32_t id, c_buffer message)
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

bool scythe::net::c_session::send_data(const BYTE* data, size_t length)
{
    if (send(socket_fd, reinterpret_cast<const char*>(data), length, 0) == SOCKET_ERROR) {
        logger::error("send failed");

        return false;
    }
    return true;
}

void scythe::net::c_session::update_state(client_state state)
{
    c_updatestate_ack updatestate_ack;

    send_message((uint32_t)message_opcode::updatestate_ack, c_updatestate_ack(state).write());
}
 