#include "c_channelcreate_req.h"

bool scythe::net::c_channelcreate_req::read(c_buffer& buffer)
{
    return buffer.read_uint64(server_id) && buffer.read_uint8((uint8_t&)type) && buffer.read_string(name);
}

scythe::net::c_buffer scythe::net::c_channelcreate_req::write()
{
    c_buffer buffer;
    buffer.write_uint64(server_id);
    buffer.write_uint8((uint8_t)type);
    buffer.write_string(name);
    return buffer;
}
