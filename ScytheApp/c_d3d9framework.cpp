#include "c_d3d9framework.h"
#include "scythe_ui.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <d3dx9.h>
scythe::c_d3d9framework* d3d9_framework = nullptr;
bool scythe::c_d3d9framework::initialize(HWND hWnd, int width, int height)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) {
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;

    if (FAILED(d3d->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
        &d3dpp, &d3dDevice))) {
        return false;
    }

    texture_cache = new c_d3d9texturecache(d3dDevice);
    present_params = d3dpp;

    return true;
}

void scythe::c_d3d9framework::clear_screen(D3DCOLOR color)
{
    if (d3dDevice) {
        d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1.0f, 0);
    }
}

void scythe::c_d3d9framework::begin_scene()
{
    if (d3dDevice) {
        d3dDevice->BeginScene();
    }
}

void scythe::c_d3d9framework::end_scene()
{
    if (d3dDevice) {
        d3dDevice->EndScene();
        d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    }
}
HRESULT scythe::c_d3d9framework::reset()
{
    if (!d3dDevice) return E_FAIL;
    return d3dDevice->Reset(&present_params);
}
bool scythe::c_d3d9framework::is_device_lost()
{
    if (!d3dDevice) return true;
    HRESULT hr = d3dDevice->TestCooperativeLevel();
    return hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET;
}
IDirect3DTexture9* scythe::c_d3d9framework::get_backbuffer_as_texture()
{
    // Get the backbuffer surface
    IDirect3DSurface9* backBufferSurface = nullptr;
    if (FAILED(d3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBufferSurface))) {
        throw std::runtime_error("Failed to retrieve backbuffer surface.");
    }

    D3DSURFACE_DESC desc;
    backBufferSurface->GetDesc(&desc);  // Get backbuffer size and format

    // Create a texture with the same format and size
    IDirect3DTexture9* texture = nullptr;
    if (FAILED(d3dDevice->CreateTexture(
        desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &texture, nullptr))) {
        backBufferSurface->Release();
        throw std::runtime_error("Failed to create texture.");
    }

    // Copy the backbuffer to the texture
    IDirect3DSurface9* textureSurface = nullptr;
    texture->GetSurfaceLevel(0, &textureSurface);

    if (FAILED(d3dDevice->GetRenderTargetData(backBufferSurface, textureSurface))) {
        textureSurface->Release();
        backBufferSurface->Release();
        texture->Release();
        throw std::runtime_error("Failed to copy backbuffer.");
    }

    // Release intermediate surfaces
    textureSurface->Release();
    backBufferSurface->Release();

    return texture;
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Forward declare message handler from imgui_impl_win32.cpp

LRESULT scythe::c_d3d9framework::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool g_dragging = false;  // Track if the window is being dragged
    static POINT g_dragStart = {};   // Store the starting point of the drag
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
    switch (message) {
    case WM_LBUTTONDOWN: {
        // Get the mouse position relative to the window
        POINT cursorPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        // Check if the click is within the draggable area (top 30 pixels)
        if (cursorPos.y <= scythe::ui::header_height) {
            // Begin dragging
            g_dragging = true;
            SetCapture(hWnd);  // Capture the mouse to track movements
            GetCursorPos(&g_dragStart);  // Store the current cursor position
        }
        return 0;
    }

    case WM_MOUSEMOVE: {
        if (g_dragging) {
            // Calculate the difference in cursor position
            POINT currentPos;
            GetCursorPos(&currentPos);

            int deltaX = currentPos.x - g_dragStart.x;
            int deltaY = currentPos.y - g_dragStart.y;

            // Get the current window position
            RECT windowRect;
            GetWindowRect(hWnd, &windowRect);

            // Move the window by the delta amount
            SetWindowPos(
                hWnd, nullptr,
                windowRect.left + deltaX,
                windowRect.top + deltaY,
                0, 0, SWP_NOZORDER | SWP_NOSIZE
            );

            // Update the starting point for the next move
            g_dragStart = currentPos;
        }
        return 0;
    }

    case WM_LBUTTONUP: {
        // Stop dragging
        g_dragging = false;
        ReleaseCapture();  // Release the mouse capture
        return 0;
    }

    case WM_SIZE:
        if (d3d9_framework && wParam != SIZE_MINIMIZED) {
            ImGui_ImplDX9_InvalidateDeviceObjects();
            if (d3d9_framework->reset() == D3DERR_INVALIDCALL) {
                // If reset fails, re-initialize ImGui device objects.
                ImGui_ImplDX9_CreateDeviceObjects();
            }
        }
        return 0;

    case WM_SYSCOMMAND:
        // Prevent the Direct3D device from being lost during system events
        if ((wParam & 0xFFF0) == SC_KEYMENU) return 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
