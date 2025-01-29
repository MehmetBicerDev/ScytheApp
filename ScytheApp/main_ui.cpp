#include "main_ui.h"
#include "server_ui.h"
#include "friend_ui.h"
#include "ui_variables.h"

#include "c_d3d9framework.h"
#include "app.h"
#include "imgui_extension.h"
#include "app_functions.h"
#include <imgui.h>

namespace scythe
{
	namespace ui
	{
		void draw_settings()
		{
			ImGui::SetWindowSize("##Popup Settings", ImVec2(500, 500));
			if (ImGui::BeginPopupModal("##Popup Settings", 0, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::SetCursorPos(ImVec2(0, 0));
				ImGui::BeginChild("##Settings.Type.Panel", ImVec2(500, 500), true);
				{

					ImGui::EndChild();
				}
				ImGui::EndPopup();
			}
		}
		void draw_server_button(const std::string& texture_name, const std::string& tooltip, c_server* server_item)
		{
			ImGui::SetCursorPosX(0);
			if (ImGui::ImageButton(d3d9_framework->texture_cache->get_cached_texture(texture_name.c_str()), ImVec2(45, 45)))
			{
				vars().selected_server = server_item;
				vars().is_friendpanel_selected = false;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("%s", tooltip.c_str());
				ImGui::EndTooltip();
			}
		}

		void draw_action_button(const char* label, const char* popup_id)
		{
			ImGui::SetCursorPosX(0);
			if (ImGui::Button(label, ImVec2(50, 20)))
			{
				ImGui::OpenPopup(popup_id);
			}
		}

	
		void draw_server_creation_popup()
		{
			if (ImGui::BeginPopupModal("ServerCreationPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImExtensions::InputTextWithHint("##ServerName", "<ServerName>", &vars().server_creation_name, 0, 0, 0);

				ImGui::Separator();
				if (ImGui::Button("Create"))
				{
					app::send_create_server(vars().server_creation_name);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		void draw_server_join_popup()
		{
			if (ImGui::BeginPopupModal("ServerJoinPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImExtensions::InputTextWithHint("##InviteID", "<InviteID>", &vars().server_join_invite, 0, 0, 0);

				ImGui::Separator();
				if (ImGui::Button("Join"))
				{
					app::send_server_join(vars().server_join_invite);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		void draw_selected_server_panel()
		{
			if (vars().is_friendpanel_selected)
			{
				draw_friend_ui();
			}
			else if (vars().selected_server)
			{
				draw_server(vars().selected_server);
			}
		}

		void draw_server_panel()
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, vars().child1_color);
			ImGui::BeginChild("ServerPanel", ImVec2(50, app::window_framework->get_height() - 22), ImGuiChildFlags_Border);
			ImGui::SetCursorPosX(0);

			ImGui::SetCursorPosY(0);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));

			if (ImGui::ImageButton(d3d9_framework->texture_cache->get_cached_texture("ScytheIcon"), ImVec2(45, 45)))
			{
				vars().selected_server = nullptr;
				vars().is_friendpanel_selected = true;
			}
			ImGui::Separator();
			// Draw Server Buttons
			for (const auto& server : app::servers)
			{
				draw_server_button("ServerDefaultIcon", server.second->name, server.second);
			}

			// Server Actions: Create and Join
			draw_action_button("Create", "ServerCreationPopup");
			draw_action_button("Join", "ServerJoinPopup");


			draw_server_creation_popup();
			draw_server_join_popup();
			draw_settings();
			ImGui::EndChild();
			ImGui::PopStyleColor(4);
		}

		void draw_user_panel()
		{
			ImGui::SetCursorPos(ImVec2(50, app::window_framework->get_height() -  57));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, vars().child1_color);

			ImGui::BeginChild("##UserPanelMainAA", ImVec2(235, 55), true);

			ImGui::Text("%s", app::own_session->account->nickname.c_str());

			if (ImGui::Button("Settings"))
			{
				ImGui::OpenPopup("##Popup Settings");
			}
			ImGui::EndChild();
			ImGui::PopStyleColor();
		}

		void draw_main_ui()
		{
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(20);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			// Draw Server Panel
			draw_server_panel();

			// Draw Popups
	


			// Draw Selected Server Panel
			ImGui::PopStyleVar();
			ImGui::SameLine(0, 0);

			draw_selected_server_panel();


			draw_user_panel();


		}
	}
}





