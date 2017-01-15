
#ifdef RANDOM_DRAW
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
	HBRUSH myBrush;
	HBRUSH oldBrush;

	switch (iMessage)
	{
	case WM_DESTROY :
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		hdc = BeginPaint(hWnd, &ps);
		RandomDraw data = RandomDraw(hWnd);

		// Hatched 스타일이었을 경우.
		if (data.m_IsStyleHatched)
		{
			myBrush = CreateHatchBrush(data.m_HatchStyleNumber, RGB(data.m_Color.red, data.m_Color.green, data.m_Color.blue));
		}
		// 아닐 경우 Solid 스타일을 만든다.
		else
		{
			myBrush = CreateSolidBrush(RGB(data.m_Color.red, data.m_Color.green, data.m_Color.blue));
		}

		// OldBrush를 저장해 둔다.
		oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

		if (data.m_DrawingType == DrawingType::RECTANGLE)
		{
			Rectangle(hdc, data.m_FirstDot.x, data.m_FirstDot.y, data.m_SecondDot.x, data.m_SecondDot.y);
		}
		else if (data.m_DrawingType == DrawingType::CIRCLE)
		{
			Ellipse(hdc, data.m_FirstDot.x, data.m_FirstDot.y, data.m_SecondDot.x, data.m_SecondDot.y);
		}
		// Polygons
		else
		{
			Polygon(hdc, data.m_PolygonDots, data.m_NumberOfPolyDots);
		}

		(HBRUSH)SelectObject(hdc, oldBrush);
		EndPaint(hWnd, &ps);

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

RandomDraw::RandomDraw(HWND hWnd)
{
	if (InitDataSetting(hWnd))
	{
		//Draw();
	}

	return;
}

bool RandomDraw::InitDataSetting(HWND hWnd)
{
	// Parameter Setting
	m_Hwnd = hWnd;

	// DrawingType Decision
	int convertTypeNum = rand() % DrawingType::TYPE_NUM;
	m_DrawingType = (DrawingType)convertTypeNum;

	// RGB Decision
	m_Color.red = rand() % 255;
	m_Color.green = rand() % 255;
	m_Color.blue = rand() % 255;

	m_IsStyleHatched = rand() % 2;
	if (m_IsStyleHatched)
	{
		m_HatchStyleNumber = rand() % HATCH_STYLE_NUM;
	}

	// If Type is not polygon
	if (m_DrawingType != DrawingType::POLYGON)
	{
		// FirstDot Decision.
		m_FirstDot.x = rand() % WIN_WIDTH;
		m_FirstDot.y = rand() % WIN_HEIGHT;

		// SecondDot Decision.
		m_SecondDot.x = rand() % WIN_WIDTH;
		m_SecondDot.y = rand() % WIN_HEIGHT;

		// Coordinates overlap
		while (m_SecondDot.x == m_FirstDot.x)
		{
			m_SecondDot.x = rand() % WIN_WIDTH;
		}

		while (m_SecondDot.y == m_FirstDot.y)
		{
			m_SecondDot.y = rand() % WIN_HEIGHT;
		}
	}

	// If Type is Polygon.
	else
	{
		m_NumberOfPolyDots = rand() % (MAX_POLYGON_DOT - 3) + 3;

		// Polygon Coordinates Decision
		for (int i = 0; i < m_NumberOfPolyDots; ++i)
		{
			m_PolygonDots[i].x = rand() % WIN_WIDTH;
			m_PolygonDots[i].y = rand() % WIN_HEIGHT;
		}

	}

	return true;
}

void RandomDraw::Draw()
{
	HDC hdc = BeginPaint(m_Hwnd, &m_Ps);
	HBRUSH myBrush;
	HBRUSH oldBrush;

	// Hatched 스타일이었을 경우.
	if (m_IsStyleHatched)
	{
		myBrush = CreateHatchBrush(m_HatchStyleNumber, RGB(m_Color.red, m_Color.green, m_Color.blue));
	}
	// 아닐 경우 Solid 스타일을 만든다.
	else
	{
		myBrush = CreateSolidBrush(RGB(m_Color.red, m_Color.green, m_Color.blue));
	}

	// OldBrush를 저장해 둔다.
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	if (m_DrawingType == DrawingType::RECTANGLE)
	{
		Rectangle(hdc, m_FirstDot.x, m_FirstDot.y, m_SecondDot.x, m_SecondDot.y);
	}
	else if (m_DrawingType == DrawingType::CIRCLE)
	{
		Ellipse(hdc, m_FirstDot.x, m_FirstDot.y, m_SecondDot.x, m_SecondDot.y);
	}
	// Polygons
	else
	{
		Polygon(hdc, m_PolygonDots, m_NumberOfPolyDots);
	}

	(HBRUSH)SelectObject(hdc, oldBrush);
	EndPaint(m_Hwnd, &m_Ps);

	return;
}


#endif
