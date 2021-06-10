#include <windows.h>
#include <gdiplus.h>

#pragma comment (lib,"gdiplus.lib")

const int HEIGHT = 612;
const int WIDTH = 816;

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR args, int nCmdShow) {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASSW wc = { 0 };
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursor(wc.hInstance, IDC_ARROW);
    wc.lpszClassName = L"newtan";
    wc.lpfnWndProc = windowProcedure;
    wc.lpszMenuName = L"NEWTAN";
    //Adds icon in corner (117 is icon)
    wc.hIcon = (HICON)LoadImageW(wc.hInstance, MAKEINTRESOURCEW(117), IMAGE_ICON, 10, 0, LR_DEFAULTSIZE | LR_SHARED);

    if (!RegisterClassW(&wc)) {
        return -1;
    }

    //Gets screen to center window
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);

    //Creates Window
    HWND hwnd = CreateWindowW(wc.lpszClassName, wc.lpszMenuName, WS_BORDER, screen.right / 2 - WIDTH / 2, screen.bottom / 2 - HEIGHT / 2, WIDTH, HEIGHT, nullptr, nullptr, wc.hInstance, nullptr);
    
    SetWindowLong(hwnd, GWL_STYLE, 0);

    ShowWindow(hwnd, SW_SHOW);


    MSG msg = { nullptr };

    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    PAINTSTRUCT ps;
    HDC hdc;
    Gdiplus::Bitmap img(GetModuleHandle(NULL), MAKEINTRESOURCE(118));
   
    switch (msg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    case WM_PAINT: {
        hdc = BeginPaint(hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);
        graphics.DrawImage(&img,0,0);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_KILLFOCUS: {
        ShowWindow(hwnd, SW_SHOWMINIMIZED);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
        SetForegroundWindow(hwnd);
        break;
    }
    default:return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}