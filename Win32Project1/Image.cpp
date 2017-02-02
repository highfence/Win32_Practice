
#ifdef IMAGE_PRACTICE
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <iostream>
#include "resource2.h"
#include <atlimage.h>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 이쪽으로 더블 버퍼링을 옮겨두자.
			// 선언은 while문 바깥에 두기 -> 불필요한 연산.
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_MOUSEMOVE :
	{
		hdc = GetDC(hWnd);
		// 더블 버퍼링.
		HDC memoryDC = CreateCompatibleDC(hdc);
		HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, 960, 840);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

		CImage img;
		img.Load(_T("../Resource/sky1.bmp"));
		img.BitBlt(memoryDC, 0, 0, 800, 600, 0, 0, SRCCOPY);

		img.Load(_T("../Resource/Background.bmp"));
		img.BitBlt(memoryDC, 0, 0, 800, 600, 0, 0, SRCCOPY);

		//HDC imgDC = CreateCompatibleDC(hdc);

		//// 첫 번째 이미지 로드.
		//HBITMAP hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1));
		//SelectObject(imgDC, hBitmap);
		//BitBlt(memoryDC, 0, 0, 960, 840, imgDC, 0, 0, SRCCOPY);

		//// 두 번째 이미지 로드.
		//hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP2));
		//SelectObject(imgDC, hBitmap);
		//BitBlt(memoryDC, 0, 0, 960, 500, imgDC, 0, 0, SRCCOPY);

		//// 한꺼번에 memoryDC에서 hdc로 올리기.
		BitBlt(hdc, 0, 0, 800, 600, memoryDC, 0, 0, SRCCOPY);

		//// 할당 해제 과정.
		//DeleteDC(imgDC);
		//DeleteObject(hBitmap);

		SelectObject(hdc, oldBitmap);
		//DeleteObject(memoryBitmap);
		DeleteDC(memoryDC);
		ReleaseDC(hWnd, hdc);
	}
		return 0;

	case WM_PAINT :
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

#endif