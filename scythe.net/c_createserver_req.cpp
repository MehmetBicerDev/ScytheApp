#include "c_createserver_req.h"

bool scythe::net::c_createserver_req::read(c_buffer& buffer)
{
    return buffer.read_string(name);
}

scythe::net::c_buffer scythe::net::c_createserver_req::write()
{
    c_buffer buffer;
    buffer.write_string(name);
    return buffer;
}
