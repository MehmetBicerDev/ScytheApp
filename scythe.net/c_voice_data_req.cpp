#include "c_voice_data_req.h"

bool scythe::net::c_voice_data_req::read(c_buffer& buffer)
{
    uint64_t length = 0;
    return buffer.read_uint64(length) && buffer.read_bytes(data, length);
}

scythe::net::c_buffer scythe::net::c_voice_data_req::write()
{
    c_buffer buffer;
    buffer.write_uint64(data.size());
    buffer.write_bytes(data, false);
    return buffer;
}
