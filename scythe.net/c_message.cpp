#include "c_message.h"

bool scythe::net::c_message::read(c_buffer& buffer)
{
    uint32_t length = 0;
    return buffer.read_uint(id) && buffer.read_uint(length) && buffer.read_bytes(data, length);
}

scythe::net::c_buffer scythe::net::c_message::write()
{
    c_buffer buffer;
    buffer.write_uint(id);
    buffer.write_uint((uint32_t)data.size());
    buffer.write_bytes(data, false);
    return buffer;
}
