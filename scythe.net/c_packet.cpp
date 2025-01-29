#include "c_packet.h"

bool scythe::net::c_packet::read(c_buffer& buffer)
{
    return (buffer.read_uint16(packet_body_id) && packet_body_id == 0x8675 && buffer.read_uint(length) && buffer.read_uint8((uint8_t&)packet_type) && buffer.read_bytes(message_buffer, length));
}

scythe::net::c_buffer scythe::net::c_packet::write()
{
    c_buffer buffer;
    buffer.write_uint16(packet_body_id);
    buffer.write_uint(length);
    buffer.write_uint8((uint8_t)packet_type);
    buffer.write_bytes(message_buffer, false);
    return buffer;
}
