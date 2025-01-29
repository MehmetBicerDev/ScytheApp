#include "c_server_userjoin_info_ack.h"

bool scythe::net::c_server_userjoin_info_ack::read(c_buffer& buffer)
{
    return buffer.read_uint64(server_id) && buffer.read_uint64(account_id) && buffer.read_string(nickname);
}

scythe::net::c_buffer scythe::net::c_server_userjoin_info_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(server_id);
    buffer.write_uint64(account_id);
    buffer.write_string(nickname);
    return buffer;
}
