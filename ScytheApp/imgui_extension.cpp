#include "imgui_extension.h"


namespace ImExtensions
{
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;

		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->data());

			// Resize the string to match the new length
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->data(); // Ensure buffer points to the updated string data
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}

		return 0;
	}

	bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = 0, void* user_data = 0)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}

	bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;

		// Allocate a writable buffer for ImGui
		return ImGui::InputTextMultiline(label, (char*)str->data(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
	}

	bool InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}
    static bool ClearInputText = false;
    struct ServerInputTextCallback_UserData
    {
        std::string* Str;
        bool* EnterPressed;
    };

    static int ServerChatInputTextCallback(ImGuiInputTextCallbackData* data)
    {
        ServerInputTextCallback_UserData* user_data = (ServerInputTextCallback_UserData*)data->UserData;
        std::string* str = user_data->Str;

        // Handle resizing (optional, can be removed if resizing is not needed)
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
         //   ImGui::SetKeyboardFocusHere(-1);
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->data();
        }
        // Detect Enter without Shift
        if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways)
        {
            if (ClearInputText)
            {
                ClearInputText = false;  // Reset the flag
                str->clear();            // Clear the string content
                data->Buf[0] = '\0';     // Set buffer to empty
                data->BufTextLen = 0;    // Update buffer length
                data->BufDirty = true;
           //     ImGui::SetKeyboardFocusHere(-1); // Refocus the input field to prevent focus loss
            }
      

   
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Enter) && !ImGui::IsKeyDown(ImGuiMod_Shift))
        {
            *user_data->EnterPressed = true;
            ClearInputText = true; // Set the flag for clearing on the next callback
        }


     
        return 0;
    }
	bool ServerChatInputText(std::string& text, const ImVec2& size)
	{
        bool pressed_enter = false;

        // Set up a buffer with enough space for editing
        char buffer[1024];
        std::strncpy(buffer, text.c_str(), sizeof(buffer) - 1); // Copy text into buffer
        buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination

        ServerInputTextCallback_UserData cb_user_data;
        cb_user_data.Str = &text;
        cb_user_data.EnterPressed = &pressed_enter;
        if (ImGui::InputTextMultiline("##ServerChatInputTextbox", buffer, sizeof(buffer), size,
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackAlways, ServerChatInputTextCallback, &cb_user_data))
        {

        }

        return pressed_enter;
    }

	void SetStyleWindowsDark()
	{
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Define Colors
        ImVec4 matte_black = ImVec4(0.07f, 0.07f, 0.07f, 1.0f);  // Matte Black
        ImVec4 dark_grey = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);    // Dark Grey
        ImVec4 mid_grey = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);     // Mid Grey
        ImVec4 accent_violet = ImVec4(0.60f, 0.20f, 0.80f, 1.0f); // Accent Violet

        // Backgrounds
        colors[ImGuiCol_WindowBg] = matte_black;
        colors[ImGuiCol_ChildBg] = dark_grey;
        colors[ImGuiCol_PopupBg] = dark_grey;
        colors[ImGuiCol_Border] = mid_grey;

        // Headers
        colors[ImGuiCol_Header] = mid_grey;
        colors[ImGuiCol_HeaderHovered] = accent_violet;
        colors[ImGuiCol_HeaderActive] = accent_violet;

        // Buttons
        colors[ImGuiCol_Button] = dark_grey;
        colors[ImGuiCol_ButtonHovered] = accent_violet;
        colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.25f, 0.85f, 1.0f);

        // Frame BG (e.g., Text Input, ComboBox)
        colors[ImGuiCol_FrameBg] = mid_grey;
        colors[ImGuiCol_FrameBgHovered] = accent_violet;
        colors[ImGuiCol_FrameBgActive] = accent_violet;

        // Tabs
        colors[ImGuiCol_Tab] = dark_grey;
        colors[ImGuiCol_TabHovered] = accent_violet;
        colors[ImGuiCol_TabActive] = ImVec4(0.70f, 0.30f, 0.85f, 1.0f);

        // Title Bar
        colors[ImGuiCol_TitleBg] = matte_black;
        colors[ImGuiCol_TitleBgActive] = accent_violet;
        colors[ImGuiCol_TitleBgCollapsed] = mid_grey;

        // Sliders
        colors[ImGuiCol_SliderGrab] = accent_violet;
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.35f, 0.90f, 1.0f);

        // Text Colors
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);  // Light grey text
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f); // Muted grey text

        // Scrollbars
        colors[ImGuiCol_ScrollbarBg] = matte_black;
        colors[ImGuiCol_ScrollbarGrab] = mid_grey;
        colors[ImGuiCol_ScrollbarGrabHovered] = accent_violet;
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.35f, 0.90f, 1.0f);

        // Checkmarks and Radios
        colors[ImGuiCol_CheckMark] = accent_violet;

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = dark_grey;
        colors[ImGuiCol_ResizeGripHovered] = accent_violet;
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.30f, 0.85f, 1.0f);

        // Separators
        colors[ImGuiCol_Separator] = mid_grey;
        colors[ImGuiCol_SeparatorHovered] = accent_violet;
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.30f, 0.85f, 1.0f);

		style.ChildRounding = 4.0f;
		style.FrameBorderSize = 1.0f;
		style.FrameRounding = 2.0f;
		style.GrabMinSize = 7.0f;
		style.PopupRounding = 2.0f;
		style.ScrollbarRounding = 12.0f;
		style.ScrollbarSize = 13.0f;
		style.TabBorderSize = 1.0f;
		style.TabRounding = 0.0f;
		style.WindowRounding = 4.0f;
	}

}