#ifdef MOUSE_PRACTICE
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MOUSE");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevhInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	WNDCLASS WndClass;
	HWND hWnd;
	MSG Message;
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
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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
	static int x;
	static int y;
	static BOOL bNowCanDraw = FALSE;


	switch (iMessage)
	{
	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN :
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		bNowCanDraw = TRUE;
		return 0;

	case WM_MOUSEMOVE :
		if (bNowCanDraw == TRUE)
		{
			hdc = GetDC(hWnd);
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			ReleaseDC(hWnd, hdc);
		}
		return 0;

	case WM_LBUTTONUP :
		bNowCanDraw = FALSE;
		return 0;

	case WM_LBUTTONDBLCLK :
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;


	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
#endif