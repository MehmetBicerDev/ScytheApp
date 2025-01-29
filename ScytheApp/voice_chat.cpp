#include "voice_chat.h"
#include <c_voice_data_req.h>
#include "app.h"

std::deque<scythe::net::c_voice_data_ack> scythe::c_voicechat::voice_data;
bool scythe::c_voicechat::is_muted = false;
bool scythe::c_voicechat::is_deafened = false;
void scythe::c_voicechat::send_voice_data(unsigned char* buffer, int size)
{
	app::client->send_message((uint32_t)net::message_opcode::voice_data_req, net::c_voice_data_req(net::c_buffer((const int8_t*)buffer, size)).write());
}
