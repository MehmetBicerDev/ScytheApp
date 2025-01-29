#include "c_channelbulkmessage_ack.h"

bool scythe::net::c_channelbulkmessage_ack::read(c_buffer& buffer)
{
    if (buffer.read_uint64(server_id) && buffer.read_uint64(channel_id))
    {
        uint16_t message_count = 0;
        if (buffer.read_uint16(message_count))
        {
            for (uint16_t i = 0; i < message_count; i++)
            {
                c_channelmessage* channelmessage = new c_channelmessage();

                if (!buffer.read_uint64(channelmessage->message_id) || !buffer.read_uint64(channelmessage->user_id) || !buffer.read_uint64(channelmessage->date_time) || !buffer.read_string(channelmessage->message)) return false;

                messages.push_back(channelmessage);
            }
            return true;
        }
    }
    return false;
}

scythe::net::c_buffer scythe::net::c_channelbulkmessage_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(server_id);
    buffer.write_uint64(channel_id);
    buffer.write_uint16(messages.size());
    for (auto message : messages)
    {
        buffer.write_uint64(message->message_id);
        buffer.write_uint64(message->user_id);
        buffer.write_uint64(message->date_time);
        buffer.write_string(message->message);
    }
    return buffer;
}
