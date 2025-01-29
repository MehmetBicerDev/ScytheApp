#include "c_voice_data_ack.h"

bool scythe::net::c_voice_data_ack::read(c_buffer& buffer)
{
    uint64_t length = 0;
    return buffer.read_uint64(id) && buffer.read_uint64(length) && buffer.read_bytes(data, length);
}

scythe::net::c_buffer scythe::net::c_voice_data_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(id);
    buffer.write_uint64(data.size());
    buffer.write_bytes(data, false);
    return buffer;
}
