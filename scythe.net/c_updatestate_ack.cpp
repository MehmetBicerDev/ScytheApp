#include "c_updatestate_ack.h"

bool scythe::net::c_updatestate_ack::read(c_buffer& buffer)
{
    return buffer.read_uint8((uint8_t&)state);
}

scythe::net::c_buffer scythe::net::c_updatestate_ack::write()
{
    c_buffer buffer;
    buffer.write_uint8((uint8_t)state);
    return buffer;
}
