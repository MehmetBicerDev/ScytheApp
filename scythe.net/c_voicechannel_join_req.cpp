#include "c_voicechannel_join_req.h"

bool scythe::net::c_voicechannel_join_req::read(c_buffer& buffer)
{
    return buffer.read_uint64(id);
}

scythe::net::c_buffer scythe::net::c_voicechannel_join_req::write()
{
    c_buffer buffer;
    buffer.write_uint64(id);
    return buffer;
}
