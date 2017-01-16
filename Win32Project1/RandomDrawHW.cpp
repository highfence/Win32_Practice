#include "RandomDrawHW.h"
#include "resource2.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("RandomDrawHW");

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

	switch (iMessage)
	{
	case WM_QUIT :
		PostQuitMessage(0);
		return 0;
		
	case WM_LBUTTONDOWN :
		auto thisData = new DrawDataSet();
		Draw(hWnd, thisData);
		break;

	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

DrawDataSet::DrawDataSet()
{
	// Color and Style Decision
	m_Color.red = rand() % 255 + 1;
	m_Color.green = rand() % 255 + 1;
	m_Color.blue = rand() % 255 + 1;

	m_IsStyleHatched = rand() % 2;
	if (m_IsStyleHatched)
	{
		m_HatchStyleNumber = rand() % HATCH_STYLE_NUM;
	}

	// Drawing Tyep Decision
	m_DrawingType = (DrawingType)(rand() % TYPE_NUM);
	if (m_DrawingType != DrawingType::POLYGON)
	{
		m_DrawingPoint.x = rand() % WIN_WIDTH;
		m_DrawingPoint.y = rand() % WIN_HEIGHT;

		m_DrawingWidth = rand() % WIN_WIDTH;
		m_DrawingHeight = rand() % WIN_HEIGHT;
	}
	else
	{
		// POLYGON must have at least three dots
		m_NumberOfPolyDots = rand() % (MAX_POLYGON_DOT - 3) + 3;

		// POLYGON Coordinates Decision
		for (int i = 0; i < m_NumberOfPolyDots; ++i)
		{
			m_PolygonDots[i].x = rand() % WIN_WIDTH;
			m_PolygonDots[i].y = rand() % WIN_HEIGHT;
		}
	}
	
}

// 윈도우 핸들과 데이터셋을 인자로 받으면 그려주는 함수.
void Draw(HWND hWnd, DrawDataSet* thisData)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = GetDC(hWnd);
	HBRUSH myBrush;

	if (thisData->m_IsStyleHatched)
	{
		myBrush = CreateHatchBrush(thisData->m_HatchStyleNumber,
			RGB(thisData->m_Color.red, thisData->m_Color.green, thisData->m_Color.blue));
	}
	else
	{
		myBrush = CreateSolidBrush(RGB(thisData->m_Color.red, thisData->m_Color.green, thisData->m_Color.blue));
	}

	// oldBrush를 저장해 놓는다.
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	if (thisData->m_DrawingType == DrawingType::RECTANGLE)
	{
		Rectangle(hdc, thisData->m_DrawingPoint.x, thisData->m_DrawingPoint.y,
			thisData->m_DrawingWidth, thisData->m_DrawingHeight);
	}
	else if (thisData->m_DrawingType == DrawingType::CIRCLE)
	{
		Ellipse(hdc, thisData->m_DrawingPoint.x, thisData->m_DrawingPoint.y,
			thisData->m_DrawingWidth, thisData->m_DrawingHeight);
	}
	else
	{
		Polygon(hdc, thisData->m_PolygonDots, thisData->m_NumberOfPolyDots);
	}

	// 저장해 놓았던 oldBrush를 복구 시킨다.
	SelectObject(hdc, oldBrush);

	ReleaseDC(hWnd, hdc);
}
