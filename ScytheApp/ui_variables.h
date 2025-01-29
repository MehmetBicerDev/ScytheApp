#pragma once
#include <imgui.h>
#include "netincludes.h"
#include "net_constants.h"
#include "c_server.h"



namespace scythe
{
	namespace ui
	{
		class c_vars
		{
		public:

			c_vars();
			~c_vars();
			static c_vars& singleton();

		public:
			static ImVec4 lerp(const ImVec4& a, const ImVec4& b, float t) {
				return ImVec4(
					a.x + (b.x - a.x) * t,
					a.y + (b.y - a.y) * t,
					a.z + (b.z - a.z) * t,
					a.w + (b.w - a.w) * t
				);
			}
			static void insert_newlines_in_text(std::string& text, float max_width_threshold)
			{
				int line_start = 0;
				for (int i = 0; i < text.size(); ++i)
				{
					float line_width = ImGui::CalcTextSize(text.c_str() + line_start, text.c_str() + i + 1).x;

					if (line_width > max_width_threshold)
					{
						text.insert(i, "\n");
						line_start = i + 1; 
						++i; 
					}
				}
			}


			//MainUI
			std::string server_creation_name = "";
			std::string server_join_invite = "";
			bool is_friendpanel_selected = true;

			//Server
			c_server* selected_server = nullptr;

			//variables
			uint64_t selected_chat_channel = 0;
			std::string chat_message = "";

			// ChannelCreation
			scythe::net::channel_type selected_channel_type = scythe::net::channel_type::voice;
			std::string channel_name = "";

			// style
			int header_height = 20;

			float child1_ratio = 0.0f;
			float child2_ratio = 0.5f;
			float child3_ratio = 1.0f;

			ImVec4 black = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
			ImVec4 dark_grey = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
			ImVec4 light_grey = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);

			ImVec4 child1_color = lerp(black, light_grey, 0.0f);
			ImVec4 child2_color = lerp(black, light_grey, 0.1f);
			ImVec4 child3_color = lerp(black, light_grey, 0.2f);


			ImFont* large_font = nullptr;
		};


		inline c_vars& vars()
		{
			return c_vars::singleton();
		}
	}
}