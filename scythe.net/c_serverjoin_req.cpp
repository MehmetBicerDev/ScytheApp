#include "c_serverjoin_req.h"

bool scythe::net::c_serverjoin_req::read(c_buffer& buffer)
{
    return buffer.read_string(invite_id);
}

scythe::net::c_buffer scythe::net::c_serverjoin_req::write()
{
    c_buffer buffer;
    buffer.write_string(invite_id);
    return buffer;
}
