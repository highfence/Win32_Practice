#ifdef WINMAIN
#include <windows.h>
#include <time.h>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

int APIENTRY WinMain( HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpszCmdParam,
					  int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize		= sizeof(WNDCLASSEX);
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon	= LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIconSm= LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;

	if( !RegisterClassEx(&wcex) )
		return 0;

	HWND	hWnd = CreateWindowEx( WS_EX_APPWINDOW
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
		, NULL );

	if( !hWnd )
		return 0;

	ShowWindow(hWnd, nCmdShow);

	MSG			msg;
	while( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam )
{
	HDC	hdc;
	PAINTSTRUCT	ps;
	UINT resultValue;

	switch(message)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		
	case WM_PAINT :

		hdc = BeginPaint(hWnd, &ps);

		Rectangle(hdc, 0, 0, 100, 100);
		Ellipse(hdc, 0, 0, 100, 100);
		TextOut(hdc, 23, 35, L"Welcome", 7);

		EndPaint(hWnd, &ps);

	case WM_LBUTTONDOWN :
		resultValue = MessageBox(NULL, L"왼쪽 버튼이 눌렸습니다.", L"L Button Down", MB_OK);

		break;

	case WM_RBUTTONDOWN :
		resultValue = MessageBox(hWnd, L"선을 그리시겠습니까?", L"R Button Down", MB_OKCANCEL);

		if (resultValue = IDOK)
		{
			hdc = GetDC(hWnd);
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, 400, 300);
			ReleaseDC(hWnd, hdc);
		}

		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif