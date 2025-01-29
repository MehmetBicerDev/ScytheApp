#pragma once
#include <imgui.h>
#include <iostream>
namespace ImExtensions
{
	void SetStyleWindowsDark();
	bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
	bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
	bool InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
	bool ServerChatInputText(std::string& text, const ImVec2& size);
}