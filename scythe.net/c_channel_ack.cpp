#include "c_channel_ack.h"

bool scythe::net::c_channel_ack::read(c_buffer& buffer)
{
    return buffer.read_uint8((uint8_t&)type) && buffer.read_string(name) && buffer.read_uint64(server_id) && buffer.read_uint64(voice_channel_id);
}

scythe::net::c_buffer scythe::net::c_channel_ack::write()
{
    c_buffer buffer;
    buffer.write_uint8((uint8_t)type);
    buffer.write_string(name);
    buffer.write_uint64(server_id);
    buffer.write_uint64(voice_channel_id);
    return buffer;
}
