#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include "resource.h"

#define _WIN32_WINNT 0x0500
#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720

#define BUTTON_ID1 0 // Start Button
#define BUTTON_ID2 1 // End Button
#define BUTTON_IDA 2 // Start Button
#define BUTTON_IDB 3 // End Button

RECT recDisplay, recWindow, recClient;

void DoGetActiveWindow();

int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_MINIMIZE);  //won't hide the window without SW_MINIMIZE
	ShowWindow(hWnd, SW_HIDE);
	DoGetActiveWindow();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	RECT rec;
	LPTSTR lptStr = _T("検定対策用ソフト");
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		TextOut(hdc, 10, 10, lptStr, lstrlen(lptStr));
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case BUTTON_ID1:
			MessageBox(hwnd, TEXT("BUTTON_ID1"), TEXT("Kitty"), MB_OK);
			break;
		case BUTTON_ID2:
			DestroyWindow(hwnd);
			break;
		case BUTTON_IDA:
			// TODO
			break;
		case BUTTON_IDB:
			// TODO
			break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

/* アクティブウィンドウのタイトルを表示する */
void DoGetActiveWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	HWND hwnd, hWnd, hDeskWnd;
	WNDCLASS winc;
	MSG msg;

	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, &recDisplay);

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WindowProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(hInstance, TEXT("MainWindow"));
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("MainWindow");

	if (!RegisterClass(&winc)) return 0;

	hwnd = CreateWindow(
		TEXT("MainWindow"),
		TEXT("検定対策ソフト"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(recDisplay.right - WINDOW_WIDTH) / 2, //左上x座標
		(recDisplay.bottom - WINDOW_HEIGHT) / 2, //左上y座標
		WINDOW_WIDTH, //幅
		WINDOW_HEIGHT, //高さ
		NULL, NULL,
		hInstance, NULL
	);
	RECT rect;
	int width = WINDOW_WIDTH;
	if (GetWindowRect(hwnd, &rect))
	{
		width = rect.right - rect.left;
	}
	width = (width / 2) - 200;
	HDC hdc = GetDC(hwnd);
	LPTSTR lptStr = _T("検定対策用ソフト");
	TextOut(hdc, 10, 10, lptStr, lstrlen(lptStr));
	CreateWindow(
		TEXT("BUTTON"),
		TEXT("START"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		300, // 基準の高さ(Y座標は前の座標に+60)
		400,
		50,
		hwnd,
		(HMENU)BUTTON_ID1,
		hInstance,
		NULL
	);

	CreateWindow(
		TEXT("BUTTON"),
		TEXT("A"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		360, // 基準の高さ(Y座標は前の座標に+60)
		195,
		50,
		hwnd,
		(HMENU)BUTTON_IDA,
		hInstance,
		NULL
	);

	CreateWindow(
		TEXT("BUTTON"),
		TEXT("B"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width + 205,
		360, // 基準の高さ(Y座標は前の座標に+60)
		195,
		50,
		hwnd,
		(HMENU)BUTTON_IDB,
		hInstance,
		NULL
	);

	CreateWindow(
		TEXT("BUTTON"),
		TEXT("CLOSE"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		width,
		420,
		400,
		50,
		hwnd,
		(HMENU)BUTTON_ID2,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOW);

	if (hwnd == NULL) return 0;

	while (GetMessage(&msg, NULL, 0, 0)) DispatchMessage(&msg);
	return msg.wParam;
}
