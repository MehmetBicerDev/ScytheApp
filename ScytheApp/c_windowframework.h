#pragma once
#include <netincludes.h>






namespace scythe
{
    class c_windowframework
    {
    public:
        c_windowframework(int w = 800, int h = 600, std::string wn = "ScytheApp") : hWnd(nullptr), width(w), height(h), window_name(wn) {}

        bool create_window(HINSTANCE hInstance, WNDPROC wndProc, DWORD style = WS_OVERLAPPEDWINDOW);

        void set_size(int w, int h);

        void set_window_name(const std::string& name);

        void set_style(DWORD style);

        HWND get_window_handle() const { return hWnd; }

        void update_window_with_bitmap(HBITMAP hBitmap);

        int get_width() { return width; }

        int get_height() { return height; }

    private:
        HWND hWnd;
        int width, height;
        std::string window_name;
	};
}