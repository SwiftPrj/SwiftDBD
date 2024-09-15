#include <iostream>
#include "driver.h"
#include "SDK.h"
#include <thread>
#include "modules.h"

HWND hGameWnd;
HWND hOverlayWnd;
RECT wndRect;

void Update() {
    while (true)
    {
        if (rendering)
        {
            if (GetAsyncKeyState(VK_F1) & 1)
                if (showmenu)
                    esp = !esp;

            if (GetAsyncKeyState(VK_F4) & 1)
                if (showmenu)
                    espBox = !espBox;

            if (GetAsyncKeyState(VK_F5) & 1)
                if (showmenu)
                    name = !name;

            if (GetAsyncKeyState(VK_F6) & 1)
                if (showmenu)
                    mate = !mate;

            if (GetAsyncKeyState(VK_LEFT) & 1)
            {
                if (showmenu)
                    if (FOV < 800)
                        FOV = FOV + 1.f;
            }
            if (GetAsyncKeyState(VK_RIGHT) & 1)
            {
                if (showmenu)
                    if (FOV > 10)
                        FOV = FOV - 1.f;
            }
        }
        if (GetAsyncKeyState(VK_INSERT) & 1)
            showmenu = !showmenu;
    }
}
static void render(FOverlay* overlay) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        overlay->begin_scene();
        overlay->clear_scene();
        frame++;
        RenderMenu();
        ESPLoop();
        RenderESP();
        overlay->end_scene();
    }
}

static void _init(FOverlay* overlay) {
    if (!overlay->window_init()) {
        printf("[!] Error init overlay window\n");
        Sleep(5000);
        return;
    }
    else {
        printf("[+] init overlay window\n");

    }

    if (!overlay->init_d2d())
        return;
    std::thread([&]() {  BaseThread2(); }).detach();
    std::thread r(render, overlay);
    std::thread up(Update);

    r.join();
    up.detach();
    overlay->d2d_shutdown();
    return;
}



int main()
{           
    if (!mem::find_driver())
    {
        std::cout << "[-] Unable to find driver" << std::endl;
        std::cin.get();
    }
    else {
        std::cout << "[+] Found Driver -> " << mem::driver_handle << std::endl;
    }

    while (!mem::find_process("DeadByDaylight-Win64-Shipping.exe"))
    {
        std::cout << "[+] Waiting for game" << std::endl;
    }

    if (mem::find_process("DeadByDaylight-Win64-Shipping.exe"))
    {
        std::cout << "[+] Found game " << mem::process_id << std::endl;
        std::cout << "[+] Getting base..." << std::endl;
        mem::process_base = mem::find_image();
        std::cout << "[+] Found base -> " << mem::process_base << std::endl;
    }
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    HDC monitor = GetDC(hDesktop);
    int current = GetDeviceCaps(monitor, VERTRES);
    int total = GetDeviceCaps(monitor, DESKTOPVERTRES);
    ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
    ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;
    g_overlay = { 0 };
    _init(g_overlay);


}  
