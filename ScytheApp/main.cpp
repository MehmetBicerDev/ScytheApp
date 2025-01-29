#include "scythe_client.h"
#include "voice_chat.h"
#include "app.h"
#include "c_windowframework.h"
#include "c_d3d9framework.h"
#include "imgui_base.h"
#include "scythe_ui.h"
#include <rand_util.h>
#include <c_register_req.h>
#include <c_serverjoin_req.h>
#include <c_voicechannel_join_req.h>
#pragma comment(lib, "scythe.net.lib")
#pragma comment(lib, "scythe.util.lib")
#pragma comment(lib, "ws2_32")

const float window_width = 1280;
const float window_height = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
    AllocConsole();

    // Redirect stdout to the console
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);

    // Redirect stderr to the console
    freopen_s(&fp, "CONOUT$", "w", stderr);

    // Redirect stdin to the console
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::cout << "Console allocated successfully!" << std::endl;
    SetConsoleTitleA("ScytheApp");

    logger::info("test");
    scythe::app::client = new scythe::net::scythe_client("93.180.132.31", 37001);

    scythe::app::window_framework = new scythe::c_windowframework(window_width, window_height);

    d3d9_framework = new scythe::c_d3d9framework();
    if (scythe::app::client->connect_to_server())
    {
        logger::info("successfully connected to server.");
        scythe::app::own_session = new scythe::net::c_session();
        scythe::app::own_session->account = new scythe::c_account();
        scythe::app::voicechat = new scythe::c_voicechat();

        scythe::app::voicechat->initialize();

        scythe::app::voicechat->startRecording();
        scythe::app::voicechat->startPlayback();


    }

    try
    {
        auto window_framework = scythe::app::window_framework;

        if (window_framework->create_window(hInstance, scythe::c_d3d9framework::WindowProc, WS_POPUP | WS_VISIBLE) && d3d9_framework->initialize(window_framework->get_window_handle(), window_width, window_height))
        {
            d3d9_framework->texture_cache->load_texture("ServerDefaultIcon", "DefaultServer.png");
            d3d9_framework->texture_cache->load_texture("ScytheIcon", "ScytheIcon.png");

            ShowWindow(window_framework->get_window_handle(), SW_SHOW);

            scythe::imgui_base::initialize();
            MSG msg = {};
            while (msg.message != WM_QUIT) {
                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else {
                    if (d3d9_framework->is_device_lost()) {
                        if (d3d9_framework->reset() == D3DERR_INVALIDCALL) {
                            continue;
                        }
                    }

                    d3d9_framework->clear_screen(D3DCOLOR_ARGB(0, 0, 0, 0));
                    d3d9_framework->begin_scene();

                    scythe::imgui_base::begin_frame();

                    scythe::ui::render();

                    scythe::imgui_base::end_frame();

                    d3d9_framework->end_scene();

     
                }
            }
        }
        else logger::error("Framework initialization failed.");
    }
    catch (std::exception ex)
    {
        printf("exception %s \n", ex.what());
    }
    
    system("pause");

    return 5;
}

