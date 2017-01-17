#ifdef RandomDrawHW
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
	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
		
	case WM_LBUTTONDOWN :
	{
		auto thisData = new DrawDataSet();
		Draw(hWnd, thisData);
		DrawDataVector.push_back(thisData);
		break;
	}
	case WM_COMMAND :
		int returnVal = -1;
		switch (LOWORD(wParam))
		{
		case ID_FILE_SAVEFILE :
			returnVal = SaveFile();
			if (returnVal != PERFORM_WELL)
			{
				MessageBoxA(NULL, "������ �߻��߽��ϴ�. �ٽ� �������ּ���.", "ERROR", MB_OK);
			}
			else
			{
				MessageBoxA(NULL, "������ �Ϸ��߽��ϴ�.", "Save", MB_OK);
			}
			break;
		case ID_FILE_LOADFILE :
			returnVal = LoadFile(hWnd);
			if (returnVal != PERFORM_WELL)
			{
				MessageBoxA(NULL, "������ �߻��߽��ϴ�. �ٽ� �ε����ּ���.", "ERROR", MB_OK);
			}
			else
			{
				MessageBoxA(NULL, "�ε带 ���½��ϴ�.", "Load", MB_OK);
			}
			break;
		}

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

// ������ �ڵ�� �����ͼ��� ���ڷ� ������ �׷��ִ� �Լ�.
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

	// oldBrush�� ������ ���´�.
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

	// ������ ���Ҵ� oldBrush�� ���� ��Ų��.
	SelectObject(hdc, oldBrush);

	ReleaseDC(hWnd, hdc);
}


int SaveFile()
{
	FILE *fp = fopen("DataSet.csv", "w");
	if (!fp)
	{
		return ERROR_FILE_OPEN;
	}

	// title
	fprintf(fp, "DrawingType,Point.x,Point.y,width,height,IsStyleHatched,HatchStyle,R,G,B,NumberOfPolyDots\n");

	// data save
	int idx;
	int dataNumber = DrawDataVector.size();

	for (idx = 0; idx < dataNumber; ++idx)
	{
		DrawDataSet* thisData = DrawDataVector.at(idx);
		fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			thisData->m_DrawingType, thisData->m_DrawingPoint.x, thisData->m_DrawingPoint.y, thisData->m_DrawingWidth, thisData->m_DrawingHeight,
			thisData->m_IsStyleHatched, thisData->m_HatchStyleNumber, thisData->m_Color.red, thisData->m_Color.green, thisData->m_Color.blue,
			thisData->m_NumberOfPolyDots);

		int polyDotNumber = thisData->m_NumberOfPolyDots;
		for (int polyIdx = 0; polyIdx < polyDotNumber; ++polyIdx)
		{
			POINT dotPoint = thisData->m_PolygonDots[polyIdx];
			fprintf(fp, ",%d,%d", dotPoint.x, dotPoint.y);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);

	return PERFORM_WELL;
}

int LoadFile(HWND hWnd)
{
	FILE *fp = fopen("DataSet.csv", "r");
	if (!fp)
	{
		return ERROR_FILE_OPEN;
	}

	// Data Vector �ʱ�ȭ.
	DrawDataVector.clear();

	char buf[1024];
	// Title ����.
	fgets(buf, 1024, fp);

	while (fgets(buf, 1024, fp))
	{
		std::string dataLine(buf);
		DrawDataSet* loadedData = new DrawDataSet;

		/*
		auto drawingType(strSplit(dataLine));
		auto drawingPoint(strSplit(dataLine));
*/
	}

	return PERFORM_WELL;
}

// ���ڿ� �տ������� ���ʷ� ������ ��ǥ������ ���ڿ��� �� �ڱ����� ���ڿ��� �߶��ִ� �Լ�.
std::string* strSplit(std::string originStr)
{
	int cutPoint;

	cutPoint = originStr.find_first_of(",");
	if (cutPoint != originStr.npos)
	{
		return nullptr;
	}

	std::string* strResult = new std::string(originStr.substr(0, cutPoint));
	originStr = originStr.substr(cutPoint + 1);

	return strResult;
}

#endif