#include "server_ui.h"
#include "app.h"
#include "ui_variables.h"
#include "imgui_extension.h"
#include "app_functions.h"
#include "c_session.h"







namespace scythe
{
	namespace ui
	{
		void draw_member_list(c_server* server)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, vars().child3_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, vars().child3_color);
			ImGui::PushStyleColor(ImGuiCol_Border, vars().child3_color);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			ImGui::SetCursorPosY(0);
			for (const auto& member : server->accounts)
			{
				ImGui::SetCursorPosX(0);
				if(ImGui::Button(member->nickname.c_str(), ImVec2(235, 45)))
				{ }
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar(2);

		}

		void draw_message_list(c_server* server)
		{
			if (vars().selected_chat_channel)
			{
				int last_user_id = -1;
				auto channel = server->messagechannels[vars().selected_chat_channel];
				if (channel)
				{
					for (const auto& message : channel->messages)
					{
						if (last_user_id != message->user_id)
						{
							if(last_user_id != -1)
								ImGui::Separator();
							ImGui::SetCursorPosX(5);
							auto account = server->get_account_by_id(message->user_id);
							if (!account) { logger::error("account does not exist (AccId:{})", message->user_id); continue; }
							ImGui::TextColored(ImVec4(1.f,1.f,0.0f,1.f),account->nickname.c_str());
							last_user_id = message->user_id;
						}
						ImGui::SetCursorPosX(5);

						auto text = message->message;
						c_vars::insert_newlines_in_text(text, app::window_framework->get_width() - 261);

						ImGui::TextWrapped(text.c_str());

					}
				}
			}
		}

		void draw_server_channels(c_server* server)
		{
			ImGui::SetCursorPosX(0);

			ImGui::PushStyleColor(ImGuiCol_Button, vars().child2_color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, vars().child2_color);
			ImGui::PushStyleColor(ImGuiCol_Border, vars().child2_color);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

			ImGui::BulletText("Chat");
			for (const auto& cm : server->messagechannels)
			{
				ImGui::SetCursorPosX(0);

				if (ImGui::Button(((vars().selected_chat_channel == cm.first ? "> " : "# ") + cm.second->name + "##" + std::to_string(cm.second->id)).c_str(), ImVec2(235, 20)))
				{
					vars().selected_chat_channel = cm.second->id;
				}
			}
			ImGui::SetCursorPosX(0);

			ImGui::BulletText("Voice");
			for (const auto& vc : server->voicechannels)
			{
				ImGui::SetCursorPosX(0);
				if (ImGui::Button(( vc.second->channel_name + "##" + std::to_string(vc.second->channel_id)).c_str(), ImVec2(235, 20)))
				{
					app::send_voicechannel_join(vc.second->channel_id);
				}

				for (auto member : vc.second->connected_sessions)
				{
					ImGui::BulletText(member->account->nickname.c_str());
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}

		void draw_channel_creation_popup(c_server* server)
		{
			const char* items[] = { "Voice", "Chat" };
			static int current_item = 0;

			if (ImGui::Combo("Type", &current_item, items, IM_ARRAYSIZE(items)))
			{
				printf("CurrentItem %d \n", current_item);
				vars().selected_channel_type = current_item == 0 ? scythe::net::channel_type::voice : scythe::net::channel_type::chat;
			}

			ImExtensions::InputText("##ChannelName", &vars().channel_name, 0, 0, 0);

			ImGui::Separator();
			if (ImGui::Button("Create"))
			{
				app::send_create_channel(server->id, vars().selected_channel_type, vars().channel_name);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
		}

		void draw_chat_panel(c_server* server)
		{
			auto channel = server->messagechannels[vars().selected_chat_channel];
			if (!channel) return;

			ImGui::BeginChild("SelectedServerChatPanel", ImVec2(app::window_framework->get_width() - 522, app::window_framework->get_height() - 22), true);
			{
				{
					ImGui::PushStyleColor(ImGuiCol_ChildBg, vars().child2_color);

					ImGui::SetCursorPosX(0);
					ImGui::BeginChild("SelectedServerChat.NamePanel", ImVec2(app::window_framework->get_width() - 522, 45));
					{
						ImGui::PushFont(vars().large_font);
						ImGui::Text(" # %s", channel->name.c_str());

						ImGui::PopFont();
						ImGui::EndChild();
					}
					ImGui::PopStyleColor();
				}
				{
					ImGui::SetCursorPos(ImVec2(0, 55));
					ImGui::BeginChild("SelectedServerChatPanel.ChatListChild", ImVec2(app::window_framework->get_width() - 522, app::window_framework->get_height() - 120), false);
					{
						draw_message_list(server);

						ImGui::EndChild();
					}
				}
				{
					ImGui::SetCursorPosX(0);
					ImGui::BeginChild("SelectedServerChatPanel.MessageBoxChild");
					{
						ImGui::SetCursorPosX(5);
						if (ImExtensions::ServerChatInputText(vars().chat_message, ImVec2(app::window_framework->get_width() - 522, 30)))
						{
							app::send_chat_message(server->id, vars().selected_chat_channel, vars().chat_message);
						}
						ImGui::EndChild();
					}
				}
				ImGui::EndChild();
			}
		}

		void draw_voicechatcontrol_panel()
		{
			ImGui::BeginChild("##VoiceChatControlPanel", ImVec2(235, 37), true);
			ImGui::SetCursorPosX(0);

			if (ImGui::Button("M/U", ImVec2(40, 15)))
			{
				scythe::c_voicechat::is_muted = !scythe::c_voicechat::is_muted;
			}
			ImGui::SameLine(0, 0);
			if (ImGui::Button("D", ImVec2(40, 15)))
			{
				scythe::c_voicechat::is_deafened = !scythe::c_voicechat::is_deafened;
			}
			ImGui::SameLine(0, 0);

			if (ImGui::Button("Leave", ImVec2(70, 15)))
			{

			}
			ImGui::EndChild();
		}

		void  draw_main_panel(c_server* server)
		{
			app::own_session->account->state.joined_voicechannel_id;
			ImGui::BeginChild("ServerMainPanel", ImVec2(235, app::window_framework->get_height() - 62 - (app::own_session->account->state.joined_voicechannel_id ? 55 : 0)), true);

			ImGui::Text(server->name.c_str());
			ImGui::Separator();
			ImGui::SetCursorPosX(0);
			ImGui::SetNextItemWidth(80.0f);

			ImExtensions::InputText("##InviteID", &server->invite_id, ImGuiInputTextFlags_ReadOnly, 0, 0);
			ImGui::SetCursorPosX(0);

			if (ImGui::Button("Create", ImVec2(80, 20)))
			{
				ImGui::OpenPopup("Create Channel");
			}
			ImGui::SetCursorPosX(0);

			if (ImGui::BeginPopupModal("Create Channel", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				draw_channel_creation_popup(server);
				ImGui::EndPopup();
			}

			draw_server_channels(server);

			ImGui::EndChild();
		}

		void draw_member_panel(c_server* server)
		{
			ImGui::BeginChild("##MemberPanel", ImVec2(235, app::window_framework->get_height() - 22), true);
			draw_member_list(server);
			ImGui::EndChild();
		}

		void draw_server(c_server* server)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, vars().child2_color);
			auto y = ImGui::GetCursorPos();


			ImGui::SetCursorPosY(app::window_framework->get_height() - 42 - 55);
			if(app::own_session->account->state.joined_voicechannel_id)
				draw_voicechatcontrol_panel();
			ImGui::SetCursorPos(y);


			draw_main_panel(server);

			ImGui::PopStyleColor();
			ImGui::SameLine(0, 0);

			if (vars().selected_chat_channel)
			{
				ImGui::PushStyleColor(ImGuiCol_ChildBg, vars().child2_color);
				draw_chat_panel(server);
				ImGui::PopStyleColor();

				ImGui::SameLine(0, 0);

				ImGui::PushStyleColor(ImGuiCol_ChildBg, vars().child3_color);
				draw_member_panel(server);
				ImGui::PopStyleColor();
			}
		}
	}
}