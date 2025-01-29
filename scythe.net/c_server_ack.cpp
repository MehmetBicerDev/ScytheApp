#include "c_server_ack.h"


bool scythe::net::c_server_ack::read(c_buffer& buffer)
{
    server = new c_server();

    uint16_t voice_channel_size = 0, account_size = 0, channel_size = 0;
    if (buffer.read_uint64(server->id) && buffer.read_string(server->name) && buffer.read_string(server->invite_id) && buffer.read_uint16(voice_channel_size))
    {
        for (uint16_t i = 0; i < voice_channel_size; i++)
        {
            c_voicechannel* voicechannel = new c_voicechannel();
            uint16_t connected_session_size = 0;

            if (!buffer.read_uint64(voicechannel->channel_id) || !buffer.read_string(voicechannel->channel_name) || !buffer.read_uint16(connected_session_size)) return false;


            for (uint16_t j = 0; j < connected_session_size; j++)
            {
                c_session* voicesession = new c_session();

                voicesession->account = new c_account();
                if (!buffer.read_uint64(voicesession->account->account_id) || !buffer.read_string(voicesession->account->nickname))
                {
                    printf("VoiceSession failure\n");
                    return false;
                }
                voicechannel->connected_sessions.push_back(voicesession);
            }

            voicechannel->owner = server;
            server->voicechannels[voicechannel->channel_id] = voicechannel;
       
        }
        if (buffer.read_uint16(channel_size))
        {
            for (int j = 0; j < channel_size; j++)
            {
                c_channel* channel = new c_channel();

                if (!buffer.read_uint64(channel->id) || !buffer.read_string(channel->name)) {  return false; }

                server->messagechannels[channel->id] = channel;
            }
            if (buffer.read_uint16(account_size))
            {
                for (uint16_t i = 0; i < account_size; i++)
                {
                    c_account* acc = new c_account();
                    if (!buffer.read_uint64(acc->account_id) || !buffer.read_string(acc->nickname)) {  return false; }

                    server->accounts.push_back(acc);
                }
            }
        }
        return true;
    }
    return false;
}

scythe::net::c_buffer scythe::net::c_server_ack::write()
{
    c_buffer buffer;
    buffer.write_uint64(server->id);
    buffer.write_string(server->name);
    buffer.write_string(server->invite_id);
    buffer.write_uint16(server->voicechannels.size());
    for (auto vc : server->voicechannels)
    {
        buffer.write_uint64(vc.second->channel_id);
        buffer.write_string(vc.second->channel_name);
        buffer.write_uint16(vc.second->connected_sessions.size());
        for (auto sess : vc.second->connected_sessions)
        {
            buffer.write_uint64(sess->account->account_id);
            buffer.write_string(sess->account->nickname);
        }
    }

    buffer.write_uint16(server->messagechannels.size());

    for (auto mc : server->messagechannels)
    {
        buffer.write_uint64(mc.second->id);
        buffer.write_string(mc.second->name);
    }

    buffer.write_uint16(server->accounts.size());

    for (auto user : server->accounts)
    {
        buffer.write_uint64(user->account_id);
        buffer.write_string(user->nickname);
    }
    return buffer;
}
