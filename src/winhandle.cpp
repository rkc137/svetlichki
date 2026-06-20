#include "winhandle.hpp"

#include "battery/embed.hpp"

LRESULT CALLBACK TrayProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                           UINT_PTR, DWORD_PTR data)
{
    *reinterpret_cast<bool*>(data) = msg == WM_APP + 1 && lp == WM_LBUTTONUP;
    return DefSubclassProc(hwnd, msg, wp, lp);
}

bool Winhandle::is_should_close()
{
    return is_closing;
}

HICON load_icon()
{
    sf::Image img{
        b::embed<"res/icon.png">().data(),
        b::embed<"res/icon.png">().size()
    };
    auto [w, h] = static_cast<sf::Vector2i>(img.getSize());

    BITMAPINFO bmi = {
        .bmiHeader = {
            .biSize = sizeof(BITMAPINFOHEADER),
            .biWidth = w,
            .biHeight = -h,
            .biPlanes = 1,
            .biBitCount = 32,
            .biCompression = BI_RGB
        }
    };

    HDC dc = GetDC(nullptr);
    void* bits;
    HBITMAP hbmp = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, &bits, 0, 0);
    ReleaseDC(nullptr, dc);

    // SFML is RGBA, WinAPI is BGRA
    const auto* src = img.getPixelsPtr();
    auto* dst = static_cast<uint8_t*>(bits);
    for(size_t i = 0; i < w * h; i++)
    {
        auto i4 = i * 4;
        dst[i4 + 0] = src[i4 + 2]; // B
        dst[i4 + 1] = src[i4 + 1]; // G
        dst[i4 + 2] = src[i4 + 0]; // R
        dst[i4 + 3] = src[i4 + 3]; // A
    }

    ICONINFO ii = {
        .fIcon = TRUE,
        .hbmMask = CreateBitmap(w, h, 1, 1, nullptr),
        .hbmColor = hbmp
    };
    HICON hIcon = CreateIconIndirect(&ii);

    DeleteObject(hbmp);
    DeleteObject(ii.hbmMask);

    return hIcon;
}

Winhandle::Winhandle(sf::RenderWindow &window, bool is_LWAA) : window(window), is_LWA_ALPHA(is_LWAA)
{
    HWND hwnd = window.getNativeHandle();
    // oreder of these calls is important for some reason
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowSubclass(hwnd, TrayProc, 0, reinterpret_cast<DWORD_PTR>(&is_closing));
    SetWindowLong(hwnd, GWL_EXSTYLE,
        (GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT) & ~WS_EX_APPWINDOW
    );
    if(!is_LWA_ALPHA)
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);

    nid = {
        .cbSize = sizeof(nid),
        .hWnd = hwnd,
        .uID = 1,
        .uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE,
        .uCallbackMessage = WM_APP + 1,
        .hIcon = load_icon()
    };
    Shell_NotifyIcon(NIM_ADD, &nid);
}

Winhandle::~Winhandle()
{
    Shell_NotifyIcon(NIM_DELETE, &nid);
    DestroyIcon(nid.hIcon);
}

void Winhandle::update_render(sf::RenderTexture& rt)
{
    auto img = rt.getTexture().copyToImage();
    auto [w, h] = static_cast<sf::Vector2i>(img.getSize());

    HDC screenDC = GetDC(nullptr);
    HDC memDC = CreateCompatibleDC(screenDC);

    BITMAPINFO bmi = {
        .bmiHeader = {
            .biSize = sizeof(BITMAPINFOHEADER),
            .biWidth = w,
            .biHeight = -h,
            .biPlanes = 1,
            .biBitCount = 32,
            .biCompression = BI_RGB
        }
    };

    void* bits;
    HBITMAP hbmp = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, &bits, 0, 0);

    // SFML is RGBA, WinAPI is BGRA
    const auto* src = img.getPixelsPtr();
    auto* dst = static_cast<uint8_t*>(bits);
    for(size_t i = 0; i < w * h; i++)
    {
        auto i4 = i * 4;
        dst[i4 + 0] = src[i4 + 2]; // B
        dst[i4 + 1] = src[i4 + 1]; // G
        dst[i4 + 2] = src[i4 + 0]; // R
        dst[i4 + 3] = src[i4 + 3]; // A
    }

    SelectObject(memDC, hbmp);

    POINT ptSrc = {0, 0};
    SIZE sz = {(LONG)w, (LONG)h};
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    UpdateLayeredWindow(
        window.getNativeHandle(),
        screenDC, nullptr, &sz, memDC, &ptSrc, 0, &blend, ULW_ALPHA
    );

    DeleteObject(hbmp);
    DeleteDC(memDC);
    ReleaseDC(nullptr, screenDC);
}