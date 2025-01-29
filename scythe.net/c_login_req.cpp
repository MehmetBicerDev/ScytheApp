#include "c_login_req.h"

bool scythe::net::c_login_req::read(c_buffer& buffer)
{
    return buffer.read_string(username) && buffer.read_string(password);
}

scythe::net::c_buffer scythe::net::c_login_req::write()
{
    c_buffer buffer;
    buffer.write_string(username);
    buffer.write_string(password);
    return buffer;
}
