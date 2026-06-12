#include "winhandle.hpp"

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

struct Winhandle::Wdata
{
    NOTIFYICONDATA nid = {};
    bool is_closing = false;
};

LRESULT CALLBACK TrayProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                           UINT_PTR, DWORD_PTR data)
{
    *reinterpret_cast<bool*>(data) = msg == WM_APP + 1 && lp == WM_LBUTTONUP;
    return DefSubclassProc(hwnd, msg, wp, lp);
}

bool Winhandle::is_should_close()
{
    return wdata->is_closing;
}

Winhandle::Winhandle(sf::RenderWindow &window) : window(window), wdata(std::make_unique<Wdata>())
{
    HWND hwnd = window.getNativeHandle();
    // oreder of these calls is important for some reason
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowSubclass(hwnd, TrayProc, 0, reinterpret_cast<DWORD_PTR>(&wdata->is_closing));
    SetWindowLong(hwnd, GWL_EXSTYLE,
        (GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT) & ~WS_EX_APPWINDOW
    );
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);

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
