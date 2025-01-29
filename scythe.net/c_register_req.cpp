#include "c_register_req.h"

bool scythe::net::c_register_req::read(c_buffer& buffer)
{
    return buffer.read_string(nickname) && buffer.read_string(email) && buffer.read_string(password);
}

scythe::net::c_buffer scythe::net::c_register_req::write()
{
    c_buffer buffer;
    buffer.write_string(nickname);
    buffer.write_string(email);
    buffer.write_string(password);
    return buffer;
}
