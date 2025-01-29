#include "c_removefriend_req.h"

bool scythe::net::c_removefriend_req::read(c_buffer& buffer)
{
    return buffer.read_string(username);
}

scythe::net::c_buffer scythe::net::c_removefriend_req::write()
{
    c_buffer buffer;
    buffer.write_string(username);
    return buffer;
}
