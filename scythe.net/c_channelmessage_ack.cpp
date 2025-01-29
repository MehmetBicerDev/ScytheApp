#include "c_channelmessage_ack.h"

bool scythe::net::c_channelmessage_ack::read(c_buffer& buffer)
{
    return buffer.read_uint64(server_id) && buffer.read_uint64(channel_id) && buffer.read_uint64(sender_id) && buffer.read_uint64(message_id) && buffer.read_string(sender_nickname) && buffer.read_string(message);
}

scythe::net::c_buffer scythe::net::c_channelmessage_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(server_id);
    buffer.write_uint64(channel_id);
    buffer.write_uint64(sender_id);
    buffer.write_uint64(message_id);
    buffer.write_string(sender_nickname);
    buffer.write_string(message);
    return buffer;
}
