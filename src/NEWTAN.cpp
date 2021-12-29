#include <windows.h>
#include <gdiplus.h>

#pragma comment (lib,"gdiplus.lib")

const int HEIGHT = 612;
const int WIDTH = 816;

Gdiplus::Bitmap* img = nullptr;

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR args, int nCmdShow) {
    //Initilzing Gdi+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    img = Gdiplus::Bitmap::FromResource(hinstance,MAKEINTRESOURCE(118));

    //initizalie wc
    WNDCLASSW wc = { 0 };
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursor(wc.hInstance, IDC_ARROW);
    wc.lpszClassName = L"newtan";
    wc.lpfnWndProc = windowProcedure;
    wc.lpszMenuName = L"NEWTAN";
    //Adds icon in corner (117 is icon)
    wc.hIcon = (HICON)LoadImageW(wc.hInstance, MAKEINTRESOURCEW(117), IMAGE_ICON, 10, 0, LR_DEFAULTSIZE | LR_SHARED);

    //see if we can register if not then stop program
    if (!RegisterClassW(&wc)) {
        return -1;
    }

    //Gets screen to center window
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);

    //Creates Window
    HWND hwnd = CreateWindowW(wc.lpszClassName, wc.lpszMenuName, WS_POPUP, screen.right / 2 - WIDTH / 2, screen.bottom / 2 - HEIGHT / 2, WIDTH, HEIGHT, nullptr, nullptr, wc.hInstance, nullptr);
    
    //Removes menu bar
    SetWindowLong(hwnd, GWL_STYLE, 0);
    ShowWindow(hwnd, SW_SHOW);

    //get message
    MSG msg = { nullptr };
    //keep getting and handling messages until no more
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    //Shutdown Gdi+
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

    switch (msg) {
        case WM_CLOSE: {
            //return 0;
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
        case WM_KILLFOCUS: {
            //When you click off it comes back to the front
            //ShowWindow(hwnd, SW_SHOWMINIMIZED);
            //ShowWindow(hwnd, SW_SHOWDEFAULT);
            //SetForegroundWindow(hwnd);
            break;
        }
        default:return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}