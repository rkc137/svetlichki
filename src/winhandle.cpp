#include "winhandle.hpp"

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK TrayProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                           UINT_PTR, DWORD_PTR data)
{
    auto* self = reinterpret_cast<Winhandle*>(data);
    if(msg == WM_APP + 1)
        if(lp == WM_LBUTTONUP)
            self->is_should_close = true;
    return DefSubclassProc(hwnd, msg, wp, lp);
}

struct Winhandle::Wdata
{
    NOTIFYICONDATA nid = {};
};

Winhandle::Winhandle(sf::RenderWindow &window) : window(window), wdata(std::make_unique<Wdata>())
{
    HWND hwnd = window.getNativeHandle();
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowLong(hwnd, GWL_EXSTYLE,
    GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW);
    SetWindowSubclass(hwnd, TrayProc, 0, reinterpret_cast<DWORD_PTR>(this));

    auto &nid = wdata.get()->nid;
    wdata->nid = {
        .cbSize = sizeof(nid),
        .hWnd = hwnd,
        .uID = 1,
        .uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE,
        .uCallbackMessage = WM_APP + 1,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION)
    };
    Shell_NotifyIcon(NIM_ADD, &nid);
}

Winhandle::~Winhandle()
{
    Shell_NotifyIcon(NIM_DELETE, &wdata->nid);
}
