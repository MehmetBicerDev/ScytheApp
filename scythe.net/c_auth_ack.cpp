#include "c_auth_ack.h"

bool scythe::net::c_auth_ack::read(c_buffer& buffer)
{
    return buffer.read_uint64(account_id) && buffer.read_string(nickname) && buffer.read_uint8((uint8_t&)result);
}

scythe::net::c_buffer scythe::net::c_auth_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(account_id);
    buffer.write_string(nickname);
    buffer.write_uint8((uint8_t)result);
    return buffer;
}
