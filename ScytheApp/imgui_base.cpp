#include "imgui_base.h"

#include "app.h"
#include "c_d3d9framework.h"
#include "imgui_extension.h"
#include "ui_variables.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

namespace scythe
{
	namespace imgui_base
	{
		void initialize()
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			ImExtensions::SetStyleWindowsDark();
			ImFontConfig config1;

			config1.SizePixels = 13.0f;
			config1.OversampleV = 3;
			config1.OversampleH = 3;
			ImFont* def_font = io.Fonts->AddFontDefault(&config1);

			ImFontConfig config;

			config.SizePixels = 16.0f; 
			config.OversampleV = 7;
			config.OversampleH = 7;
			config.PixelSnapH = true;
			config.RasterizerMultiply = 1.5f;

			ImFont* large_font = io.Fonts->AddFontDefault(&config);

			ui::vars().large_font = large_font;
			auto style = ImGui::GetStyle();

			ImGui_ImplWin32_Init(app::window_framework->get_window_handle());
			ImGui_ImplDX9_Init(d3d9_framework->get_device());
		}
		void begin_frame()
		{
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}
		void end_frame()
		{
			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}
	}
}