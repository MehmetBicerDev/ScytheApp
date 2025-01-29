#include "c_voicechannel_join_ack.h"

bool scythe::net::c_voicechannel_join_ack::read(c_buffer& buffer)
{
    return buffer.read_uint64(channel_id) && buffer.read_uint64(user_id) && buffer.read_string(nickname);
}

scythe::net::c_buffer scythe::net::c_voicechannel_join_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(channel_id);
    buffer.write_uint64(user_id);
    buffer.write_string(nickname);
    return buffer;
}
