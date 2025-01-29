#include "c_voicechannel_leave_ack.h"

bool scythe::net::c_voicechannel_leave_ack::read(c_buffer& buffer)
{
    return buffer.read_uint64(channel_id) && buffer.read_uint64(user_id);
}

scythe::net::c_buffer scythe::net::c_voicechannel_leave_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(channel_id);
    buffer.write_uint64(user_id);
    return buffer;
}
