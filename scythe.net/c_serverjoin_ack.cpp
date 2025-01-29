#include "c_serverjoin_ack.h"

bool scythe::net::c_serverjoin_ack::read(c_buffer& buffer)
{
    return buffer.read_uint8((uint8_t&)result);
}

scythe::net::c_buffer scythe::net::c_serverjoin_ack::write()
{
    c_buffer buffer;
    buffer.write_uint8((uint8_t)result);
    return buffer;
}
