#include "scythe_ui.h"
#include "app.h"
#include "server_ui.h"
#include "main_ui.h"
#include "app_functions.h"
#include "c_d3d9framework.h"
#include "imgui_extension.h"
#include <imgui.h>
#include "voice_chat.h"
#include <c_voicechannel_join_req.h>
#include <c_register_req.h>
#include <c_createserver_req.h>
#include <c_channelcreate_req.h>
#include <c_serverjoin_req.h>
#include <rand_util.h>
#include <c_login_req.h>

namespace scythe
{
	namespace ui
	{
		ImVec4 lerp(const ImVec4& a, const ImVec4& b, float t) {
			return ImVec4(
				a.x + (b.x - a.x) * t,
				a.y + (b.y - a.y) * t,
				a.z + (b.z - a.z) * t,
				a.w + (b.w - a.w) * t
			);
		}
		int header_height = 20;  
		const float child1_ratio = 0.0f;
		const float child2_ratio = 0.5f;
		const float child3_ratio = 1.0f;
		ImVec4 black = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);         // Black
		ImVec4 dark_grey = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);     // Dark Grey
		ImVec4 light_grey = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);    // Brighter Grey

		// Interpolated colors for each child window
		ImVec4 child1_color = lerp(black, light_grey, 0.0f);  // Darkest (Black)
		ImVec4 child2_color = lerp(black, light_grey, 0.1f);  // Midpoint (Dark Grey)
		ImVec4 child3_color = lerp(black, light_grey, 0.2f);  // Brightest (Light Grey)

		void render_auth_window();
		void render_main_window();
		void render()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);  // Rounds the entire window
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f); // Adds a border to make rounding more noticeable
			ImGui::SetWindowPos("ScytheApp", ImVec2(0, 0));
			ImGui::SetWindowSize("ScytheApp", ImVec2(app::window_framework->get_width(), app::window_framework->get_height()));
			ImGui::Begin("ScytheApp", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			if (!app::login_success)
				render_auth_window();
			else if(app::client_state == net::client_state::client_loaded)
				draw_main_ui();
			ImGui::End();

			ImGui::PopStyleVar(2);
		}

		void insert_newlines_in_text(std::string& text, float max_width_threshold)
		{
			int line_start = 0;

			for (int i = 0; i < text.size(); ++i)
			{
				// Calculate width of current line segment
				float line_width = ImGui::CalcTextSize(text.c_str() + line_start, text.c_str() + i + 1).x;

				if (line_width > max_width_threshold)
				{
					// Insert newline at the current position
					text.insert(i, "\n");
					line_start = i + 1; // Update line start to the next character after newline
					++i; // Move past the newly inserted newline
				}
			}
		}


		void render_auth_window()
		{
			static bool login_menu = true;
			static std::string email = "";
			static std::string password = "";
			static std::string nickname = "";

			ImVec2 window_size = ImGui::GetWindowSize();
			ImVec2 window_pos = ImGui::GetWindowPos();

			ImVec2 child_size(300.0f, 250.0f);

			float child_pos_x = window_pos.x + (window_size.x - child_size.x) / 2.0f;
			float child_pos_y = window_pos.y + (window_size.y - child_size.y) / 2.0f;
			float element_width = 180.0f;
			float element_pos_x = (child_size.x - element_width) / 2.0f;
			ImGui::SetCursorPos(ImVec2(child_pos_x - window_pos.x, child_pos_y - window_pos.y));
			ImGui::BeginChild("AuthPanel", child_size, true);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);  
			if (login_menu)
			{
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SetCursorPosY(50.0f);
				ImGui::SetCursorPosX(element_pos_x);
				ImExtensions::InputTextWithHint("##EmailText", "example@gmail.com", &email, 0, 0, 0);
				ImGui::Spacing();
				ImGui::SetCursorPosX(element_pos_x);
				ImExtensions::InputTextWithHint("##PasswordText","**********", &password, ImGuiInputTextFlags_Password, 0, 0);
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SetCursorPosX(element_pos_x);
				ImGui::Text(app::auth_result_message.c_str());
				ImGui::Spacing();

				ImGui::SetCursorPosX(element_pos_x);
				if (ImGui::Button("Login", ImVec2(element_width + 14.0f, 0)))
				{
					app::own_session->account->email = email;
					app::own_session->account->password = password;
					scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::login_req, scythe::net::c_login_req(email, password).write());
				}
				ImGui::Spacing();

				auto size = ImGui::CalcTextSize("Click here to register.");
				ImGui::SetCursorPosX(size.x / 2.f);
				ImGui::Text("Click here to register.");

				if (ImGui::IsItemClicked())
					login_menu = false;

			}
			else
			{
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SetCursorPosY(50.0f);
				ImGui::SetCursorPosX(element_pos_x);
				ImExtensions::InputTextWithHint("##EmailText", "example@gmail.com", &email, 0, 0, 0);
				ImGui::Spacing();
				ImGui::SetCursorPosX(element_pos_x);
				ImExtensions::InputTextWithHint("##PasswordText", "**********", &password, ImGuiInputTextFlags_Password, 0, 0);
				ImGui::Spacing();
				ImGui::SetCursorPosX(element_pos_x);
				ImExtensions::InputTextWithHint("##NicknameText", "nickname", &nickname, 0, 0, 0);
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SetCursorPosX(element_pos_x);
				ImGui::Text(app::auth_result_message.c_str());
				ImGui::Spacing();

				ImGui::SetCursorPosX(element_pos_x);
				if (ImGui::Button("Register", ImVec2(element_width + 14.0f, 0)))
				{
					app::own_session->account->email = email;
					app::own_session->account->password = password;
					app::own_session->account->nickname = nickname;
					scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::register_req, scythe::net::c_register_req(nickname, email, password).write());

				}
				ImGui::Spacing();

				auto size = ImGui::CalcTextSize("Click here to login.");
				ImGui::SetCursorPosX(size.x / 2.f);
				ImGui::Text("Click here to login.");

				if (ImGui::IsItemClicked())
					login_menu = true;

			}

			ImGui::PopStyleVar();
			ImGui::EndChild();

		}

		void render_main_window()
		{
			static std::string server_name = "";
			static std::string server_join_invite = "";
			static c_server* selected_server = nullptr;
			ImGui::SetCursorPosX(0);

			ImGui::SetCursorPosY(20);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			ImGui::PushStyleColor(ImGuiCol_ChildBg, child1_color); // Apply color for Child 1
			ImGui::BeginChild("ServerPanel", ImVec2(50, app::window_framework->get_height() - 22), ImGuiChildFlags_Border);

			ImGui::SetCursorPosY(0);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));

			for (auto server : app::servers)
			{
				auto server_item = server.second;
				ImGui::SetCursorPosX(0);

				if(ImGui::ImageButton(d3d9_framework->texture_cache->get_cached_texture("ServerDefaultIcon"), ImVec2(45, 45)))
				{
					selected_server = server_item;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();  
					ImGui::Text(server_item->name.c_str());
					ImGui::EndTooltip();  
				}
		
			
				
			}

			ImGui::SetCursorPosX(0);
			if(ImGui::Button("Create Server", ImVec2(50, 20)))
			{
				ImGui::OpenPopup("ServerCreationPopup");  // Open the popup
			}
			ImGui::SetCursorPosX(0);
			if (ImGui::Button("Join Server", ImVec2(50, 20)))
			{
				ImGui::OpenPopup("ServerJoinPopup");  // Open the popup
			}
			if (ImGui::BeginPopupModal("ServerCreationPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImExtensions::InputTextWithHint("##ServerName", "<ServerName>", &server_name, 0, 0, 0);


				ImGui::Separator();
				if (ImGui::Button("Create"))
				{
					scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::createserver_req, scythe::net::c_createserver_req(server_name).write());
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();  // Close the popup
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("ServerJoinPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImExtensions::InputTextWithHint("##InviteID", "<InviteID>", &server_join_invite, 0, 0, 0);


				ImGui::Separator();
				if (ImGui::Button("Join"))
				{
					scythe::app::client->send_message((uint32_t)scythe::net::message_opcode::serverjoin_req, scythe::net::c_serverjoin_req(server_join_invite).write());
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();  // Close the popup
				}

				ImGui::EndPopup();
			}

			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor(4);

			ImGui::SameLine(0, 0);


			if (selected_server)
			{
				draw_server(selected_server);
				//render_server(selected_server);
			}
			else
			{
				//render friends
			}
		}
		scythe::net::channel_type selected_channel_type = scythe::net::channel_type::none;
		uint64_t selected_chat_channel = 0;
	}
}