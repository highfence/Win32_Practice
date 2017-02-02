#ifdef CURSOR
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <iostream>
#include "resource2.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//HCURSOR LoadAnimatedCursor(HINSTANCE hInstance, UINT nID, LPCTSTR pszResouceType)
//{
//	HCURSOR hCursor = NULL;
//	if (hInstance)
//	{
//		HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(nID), pszResouceType);
//		DWORD dwResourceSize = SizeofResource(hInstance, hResource);
//		if (dwResourceSize>0)
//		{
//			HGLOBAL hRsrcGlobal = LoadResource(hInstance, hResource);
//			if (hRsrcGlobal)
//			{
//				LPBYTE pResource = (LPBYTE)LockResource(hRsrcGlobal);
//				if (pResource)
//				{
//					hCursor = (HCURSOR)CreateIconFromResource(pResource, dwResourceSize, FALSE, 0x00030000);
//					UnlockResource(pResource);
//				}
//				FreeResource(hRsrcGlobal);
//			}
//		}
//	}
//	return hCursor;
//}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
		return 0;

	HWND	hWnd = CreateWindowEx(WS_EX_APPWINDOW
		, szWindowClass
		, szTitle
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, 800
		, 600
		, NULL
		, NULL
		, hInstance
		, NULL);

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow);

	MSG			msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	TCHAR buf[255];


	switch (iMessage)
	{
	case WM_MOUSEMOVE :
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		Rectangle(hdc, x - 10, y - 10, x + 10, y + 10);
		wsprintf(buf, TEXT("X : %d Y : %d"), x, y);
		TextOut(hdc, 100, 100, buf, lstrlen(buf));
		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONDOWN :
		hdc = GetDC(hWnd);
		Rectangle(hdc, x - 20, y - 20, x + 20, y + 20);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_PAINT :
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
#endif