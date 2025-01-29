#include "c_windowframework.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

bool scythe::c_windowframework::create_window(HINSTANCE hInstance, WNDPROC wndProc, DWORD style)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ScytheApp";

    RegisterClass(&wc);

    hWnd = CreateWindowEx(
        WS_EX_LAYERED, "ScytheApp", window_name.c_str(), style,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return false;
    }       
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    return true;
}

void scythe::c_windowframework::set_size(int w, int h)
{
    width = w;
    height = h;
    if (hWnd) {
        SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void scythe::c_windowframework::set_window_name(const std::string& name)
{
    window_name = name;
    if (hWnd) {
        SetWindowText(hWnd, window_name.c_str());
    }
}

void scythe::c_windowframework::set_style(DWORD style)
{
    SetWindowLongPtr(hWnd, GWL_STYLE, style);
    SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void scythe::c_windowframework::update_window_with_bitmap(HBITMAP hBitmap)
{
    HDC hdcScreen = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    SelectObject(hdcMem, hBitmap);

    POINT ptZero = { 0, 0 };
    SIZE sizeWindow = { width, height };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    if (!UpdateLayeredWindow(hWnd, hdcScreen, nullptr, &sizeWindow, hdcMem,
        &ptZero, RGB(0, 0, 0), &blend, ULW_ALPHA)) {
        throw std::runtime_error("Failed to update layered window.");
    }

    DeleteDC(hdcMem);
    ReleaseDC(nullptr, hdcScreen);
}
