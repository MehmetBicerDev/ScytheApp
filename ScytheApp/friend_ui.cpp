#include "friend_ui.h"

#include "app.h"
#include "ui_variables.h"
namespace scythe
{
	namespace ui
	{
		void draw_main_panel()
		{
			ImGui::BeginChild("FriendMainPanel", ImVec2(235, app::window_framework->get_height() - 62), true);
			{
				ImGui::EndChild();
			}
		}
		void draw_friend_ui()
		{
			draw_main_panel();
		}
	}
}