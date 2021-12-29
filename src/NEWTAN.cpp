#include <windows.h>
#include <gdiplus.h>

#pragma comment (lib,"gdiplus.lib")

Gdiplus::Bitmap* img = nullptr;

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR args, int nCmdShow) {
    //Initilzing Gdi+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    //118 is the image resoruce
    img = Gdiplus::Bitmap::FromResource(hinstance,MAKEINTRESOURCE(118));

    //initizalie wc
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = (HBRUSH)0;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursor(wc.hInstance, IDC_ARROW);
    wc.lpszClassName = L"newtan";
    wc.lpfnWndProc = windowProcedure;
    wc.lpszMenuName = L"NEWTAN";
    //Adds icon for taskbar (117 is icon)
    wc.hIcon = (HICON)LoadImageW(wc.hInstance, MAKEINTRESOURCEW(117), IMAGE_ICON, 10, 0, LR_DEFAULTSIZE | LR_SHARED);

    //see if we can register if not then stop program
    if (!RegisterClassEx(&wc)) {
        return -1;
    }

    //Gets screen to center window
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);

    //Creates Window
    HWND hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,wc.lpszClassName, wc.lpszMenuName, WS_POPUP, screen.right / 2 - img->GetWidth() / 2, screen.bottom / 2 - img->GetHeight() / 2, img->GetWidth(), img->GetHeight(), nullptr, nullptr, wc.hInstance, nullptr);
    
    //Removes menu bar
    SetWindowLong(hwnd, GWL_STYLE, 0);
    ShowWindow(hwnd, SW_SHOW);

    //need to call this function for layered window
    SetLayeredWindowAttributes(hwnd,0,0,NULL);

    //get message
    MSG msg = { nullptr };
    //keep getting and handling messages until no more
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    //Shutdown Gdi+
    Gdiplus::GdiplusShutdown(gdiplusToken);
    delete img;
    return 0;
}

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

    switch (msg) {
        case WM_CLOSE: {
            //don't do anything when they try to close (a little trolling)
            return 0;
        }
        case WM_PAINT: {
            //initizle ps and hdc
            PAINTSTRUCT ps;
            //start painting
            HDC hdc = BeginPaint(hwnd, &ps);
            //make graphics
            Gdiplus::Graphics graphics(hdc);
            //draw the actual image
            graphics.DrawImage(img, 0, 0);
            //stop painting
            EndPaint(hwnd, &ps);
            break;
        }
        default:return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}