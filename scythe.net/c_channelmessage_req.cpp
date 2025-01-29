#include "c_channelmessage_req.h"

bool scythe::net::c_channelmessage_req::read(c_buffer& buffer)
{
    return buffer.read_uint64(server_id) && buffer.read_uint64(channel_id) && buffer.read_string(message);
}

scythe::net::c_buffer scythe::net::c_channelmessage_req::write()
{
    c_buffer buffer;
    buffer.write_uint64(server_id);
    buffer.write_uint64(channel_id);
    buffer.write_string(message);
    return buffer;
}
