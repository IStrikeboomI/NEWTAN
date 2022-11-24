#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <random>

#pragma comment (lib,"gdiplus.lib")

//constant for timer's code
static constexpr int UPDATE_TIMER = 12;

//used for scaling the image to the screen dimensions
static constexpr int BASE_SCREEN_WIDTH = 1920, BASE_SCREEN_HEIGHT = 1080;
float screenWidth = 0, screenHeight = 0;

//the cat image
Gdiplus::Bitmap* img;

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int main() {
	//Initilzing Gdi+
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	//This should auto delete
	img = Gdiplus::Bitmap::FromResource(GetModuleHandle(nullptr), MAKEINTRESOURCE(118));

	//initizalie wc
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"newtan";
	wc.lpfnWndProc = windowProcedure;
	wc.lpszMenuName = L"NEWTAN";
	//Adds icon in corner (117 is icon)
	wc.hIcon = static_cast<HICON>(LoadImageW(wc.hInstance, MAKEINTRESOURCEW(117), IMAGE_ICON, 256, 191, LR_DEFAULTSIZE | LR_SHARED));

	//see if we can register if not then stop program
	if (!RegisterClassW(&wc)) {
		return -1;
	}

	//Gets screen to center window
	RECT screen;
	GetWindowRect(GetDesktopWindow(), &screen);

	screenWidth = screen.right;
	screenHeight = screen.bottom;

	//Creates Window
	HWND hwnd = CreateWindowW(wc.lpszClassName, wc.lpszMenuName, WS_POPUP,
						 screenWidth / 2 - (img->GetWidth() * (screenWidth / BASE_SCREEN_WIDTH)) / 2, 
						 screenHeight / 2 - (img->GetHeight() * (screenHeight / BASE_SCREEN_HEIGHT)) / 2,
						 img->GetWidth() * (screenWidth / BASE_SCREEN_WIDTH), 
						 img->GetHeight() * (screenHeight / BASE_SCREEN_HEIGHT), nullptr, nullptr, wc.hInstance, nullptr);

	//Removes menu bar
	SetWindowLong(hwnd, GWL_STYLE, 0);
	ShowWindow(hwnd, SW_SHOW);

	//Set a timer to check every 16 ms if task manager is open
	SetTimer(hwnd, UPDATE_TIMER, 16, nullptr);

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
			return 0;
		}
		case WM_TIMER: {
			switch (wparam) {
				case UPDATE_TIMER: {
					//if win tab screen is open, minimize it
					HWND winTabScreen = FindWindowA(nullptr, "Task View");
					if (winTabScreen) {
						CloseWindow(winTabScreen);
					}
					char buf[32];
					GetWindowTextA(GetForegroundWindow(), buf, 32);
					std::string windowName(buf);
					if (FindWindowA(nullptr,"Task Manager") || windowName != "NEWTAN") {
						std::random_device dev;
						std::mt19937 rng(dev());
						//distribution in range of the screen dimensions
						std::uniform_int_distribution<std::mt19937::result_type> widthDistribution(0, screenWidth);
						std::uniform_int_distribution<std::mt19937::result_type> heightDistribution(0, screenHeight);
						SetCursorPos(widthDistribution(rng),heightDistribution(rng));
						//show newtan again
						ShowWindow(hwnd, SW_SHOWMINIMIZED);
						ShowWindow(hwnd, SW_SHOWDEFAULT);
						SetForegroundWindow(hwnd);
					}
				}
			}
		}
		case WM_ERASEBKGND:{
			//don't handle WM_ERASEBKGND to prevent flickering
			return 1;
		}
		case WM_PAINT: {
			//initizle ps and hdc
			PAINTSTRUCT ps;
			//start painting
			HDC hdc = BeginPaint(hwnd, &ps);
			//make graphics
			Gdiplus::Graphics graphics(hdc);
			//draw the actual image
			graphics.DrawImage(img, 0, 0, static_cast<int>(img->GetWidth() * (screenWidth / BASE_SCREEN_WIDTH)), static_cast<int>(img->GetHeight() * (screenHeight / BASE_SCREEN_HEIGHT)));
			//stop painting
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_KILLFOCUS: {
			//When you click off it comes back to the front
			ShowWindow(hwnd, SW_SHOWMINIMIZED);
			ShowWindow(hwnd, SW_SHOWDEFAULT);
			SetForegroundWindow(hwnd);
			break;
		}
		case WM_KEYDOWN: {
			//Make escape code esc + 8 + a
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				if (GetAsyncKeyState(0x38) & 0x8000) {
					if (GetAsyncKeyState(0x41) & 0x8000) {
						ExitProcess(0);
					}
				}
			}
			break;
		}
		default:return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}