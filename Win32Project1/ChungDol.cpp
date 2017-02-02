#ifdef CHUNGDOL
#include <Windows.h>
#include <atlimage.h>
#include "ChungDol.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevhInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;
	HWND hWnd;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;


	switch (iMessage)
	{
	case WM_PAINT :
	{
		hdc = BeginPaint(hWnd, &ps);
		PaintProc(hWnd, hdc, ps);
		EndPaint(hWnd, &ps);
	}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MOUSEMOVE:
	{
		hdc = GetDC(hWnd);
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		isMouseOnRect = CheckMouseIsOnRect();
		ReleaseDC(hWnd, hdc);

		// WM_PAINT 강제 실행.
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
	return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}


void DrawRect(HWND hWnd, HDC hdc, HBRUSH myBrush, HBRUSH oldBrush)
{
	using namespace RectValues;
	myBrush = CreateSolidBrush(RGB(0, 100, 200));
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	Rectangle(hdc, rectX1, rectY1, rectX2, rectY2);
	SetTextAlign(hdc, TA_CENTER);
	TextOut(hdc, 450, 450, infoStr, lstrlen(infoStr));
	return;
}


void DrawImg(HDC hdc)
{
	CImage img;
	img.Load(_T("../Resource/Collision.bmp"));
	img.BitBlt(hdc, mouseX, mouseY, 300, 300, 0, 0, SRCCOPY);
	return;
}


void PaintProc(HWND hWnd, HDC hdc, PAINTSTRUCT ps)
{
	HBRUSH myBrush = (HBRUSH)NULL;
	HBRUSH oldBrush = (HBRUSH)NULL;

	DrawRect(hWnd, hdc, myBrush, oldBrush);

	if (isMouseOnRect == true)
	{
		DrawImg(hdc);
	}

	SelectObject(hdc, oldBrush);
	return;
}


bool CheckMouseIsOnRect()
{
	using namespace RectValues;
	if ((mouseX < rectX1) || (mouseX > rectX2))
	{
		return false;
	}
	else if ((mouseY < rectY1) || (mouseY > rectY2))
	{
		return false;
	}

	return true;
}

#endif