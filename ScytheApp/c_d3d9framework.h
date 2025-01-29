#pragma once
#include <netincludes.h>
#include <d3d9.h>
#include "c_d3d9texturecache.h"
#pragma comment(lib, "d3d9")



namespace scythe
{
    class c_d3d9framework {
    public:
        c_d3d9framework() : d3d(nullptr), d3dDevice(nullptr) {}

        bool initialize(HWND hWnd, int width, int height);

        void clear_screen(D3DCOLOR color);

        void begin_scene();

        void end_scene();

        HRESULT reset();

        bool is_device_lost();

        IDirect3DTexture9* get_backbuffer_as_texture();

        ~c_d3d9framework() {
            if (d3dDevice) d3dDevice->Release();
            if (d3d) d3d->Release();
        }

        IDirect3DDevice9* get_device() const { return d3dDevice; }

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


        c_d3d9texturecache* texture_cache = nullptr;
    private:   
        D3DPRESENT_PARAMETERS present_params;

        IDirect3D9* d3d;
        IDirect3DDevice9* d3dDevice;

        int width = 800;
        int height = 600;
    };
}

extern scythe::c_d3d9framework* d3d9_framework;