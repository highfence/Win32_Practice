#include <Windows.h>
#include "resource2.h"
#include "RandomDraw.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("RandomDraw");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&WndClass))
	{
		return 0;
	}

	HWND hWnd;
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIN_WIDTH, WIN_HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);

	if (!hWnd)
	{
		return 0;
	}

	char text[256];
	LoadStringA(hInstance, IDS_TITLE, text, 256);
	SetWindowTextA(hWnd, text);

	ShowWindow(hWnd, nCmdShow);
	
	MSG Message;
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
	case WM_DESTROY :
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		hdc = GetDC(hWnd);

		int drawType = rand() % 2;
		int FirstDotX = rand() % 800;
		int FirstDotY = rand() % 600;
		int SecondDotX = rand() % (FirstDotX + 1);
		int SecondDotY = rand() % (FirstDotY + 1);

		HBRUSH myBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(rand() % 255, rand() % 255, rand() % 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

		if (drawType == 0)
		{
			Rectangle(hdc, FirstDotX, FirstDotY, SecondDotX, SecondDotY);
		}
		else
		{
			Ellipse(hdc, FirstDotX, FirstDotY, SecondDotX, SecondDotY);
		}

		SelectObject(hdc, oldBrush);

		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_PAINT:
	{
	/*	
		hdc = BeginPaint(hWnd, &ps);

		HBRUSH myBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		Rectangle(hdc, 0, 0, 100, 100);

		myBrush = CreateSolidBrush(RGB(255, 0, 0));
		(HBRUSH)SelectObject(hdc, myBrush);
		POINT tryangle[3] = {100, 100, 150, 0, 200, 100};
		Polygon(hdc, tryangle, 3);

		myBrush = CreateSolidBrush(RGB(255, 255, 0));
		(HBRUSH)SelectObject(hdc, myBrush);
		Ellipse(hdc, 200, 0, 300, 100);

		POINT star[5] = {100, 100, 150, 250, 0, 150, 200, 150, 50, 250};
		Polygon(hdc, star, 5);

		myBrush = CreateSolidBrush(RGB(200, 0, 255));
		(HBRUSH)SelectObject(hdc, myBrush);
		POINT hexagon[6] = {400, 400, 450, 420, 450, 470, 400, 490, 350, 470, 350, 420};
		Polygon(hdc, hexagon, 6);

		(HBRUSH)SelectObject(hdc, oldBrush);
		EndPaint(hWnd, &ps);
		break;
*/		  
	}
	case WM_COMMAND :
	{
		switch ( LOWORD(wParam) )
		{
		case ID_FILE_SAVEFILE :
			MessageBox(NULL, L"저장하시겠습니까?", L"Save", MB_OKCANCEL);
			break;

		default:
			break;
		}
	}
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

RandomDraw::RandomDraw()
{
	if (InitDataSetting())
	{
		Draw();
	}

	return;
}

bool RandomDraw::InitDataSetting()
{
	// DrawingType Decision
	m_DrawingType = (DrawingType)rand();

	// If Type is not polygon
	if (m_DrawingType != DrawingType::POLYGON)
	{
		// FirstDot Decision.
		m_FirstDot.x = rand() % WIN_WIDTH;
		m_FirstDot.y = rand() % WIN_HEIGHT;

		// SecondDot Decision.
	}

	// If Type is Polygon.
	else
	{

	}

	return true;
}



