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
		DrawDataVector.push_back(thisData);
		Draw(hWnd, thisData);
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
				MessageBoxA(NULL, "에러가 발생했습니다. 다시 저장해주세요.", "ERROR", MB_OK);
			}
			else
			{
				MessageBoxA(NULL, "저장을 완료했습니다.", "Save", MB_OK);
			}
			break;
			
		case ID_FILE_LOADFILE :
			returnVal = LoadFile(hWnd);
			if (returnVal != PERFORM_WELL)
			{
				MessageBoxA(NULL, "에러가 발생했습니다. 다시 로드해주세요.", "ERROR", MB_OK);
			}
			else
			{
				MessageBoxA(NULL, "로드를 끝냈습니다.", "Load", MB_OK);
				DrawVector(hWnd);
			}
			break;
			
		case ID_FILE_REFRESH :
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_FILE_EXIT :
			DestroyWindow(hWnd);
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

	int randomVal = rand() % 2;
	if (randomVal == 0)
	{
		m_IsStyleHatched = false;
	}
	else
	{
		m_IsStyleHatched = true;
	}

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
	
	return;
}

// 윈도우 핸들과 데이터셋을 인자로 받으면 그려주는 함수.
void Draw(HWND hWnd, DrawDataSet* thisData)
{
	HDC hdc;

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

	return;
}

// DrawDataVector에 들어있는 모든 도형을 그려주는 함수.
void DrawVector(HWND hWnd)
{
	for (auto i : DrawDataVector)
	{
		Draw(hWnd, i);
	}

	return;
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
		// 저장을 위한 변수 선언.
		DrawingType inputDrawingType;
		POINT inputDrawingPoint;
		int inputDrawingWidth = 0;
		int inputDrawingHeight = 0;
		int inputNumberOfPolyDots = 0;
		POINT inputPolygonDots[MAX_POLYGON_DOT] = { 0 };
		RGB_t inputColor;
		bool inputIsStyleHatched;
		int inputHatchStyleNumber;

		DrawDataSet* thisData = DrawDataVector.at(idx);

		// DrawingType에 의한 분류.
		inputDrawingType = thisData->m_DrawingType;
		if (inputDrawingType != POLYGON)
		{
			inputDrawingPoint = thisData->m_DrawingPoint;
			inputDrawingWidth = thisData->m_DrawingWidth;
			inputDrawingHeight = thisData->m_DrawingHeight;

			inputNumberOfPolyDots = INT_NULL;
			
		}
		else
		{
			inputDrawingPoint.x = INT_NULL;
			inputDrawingPoint.y = INT_NULL;
			inputDrawingWidth = INT_NULL;
			inputDrawingHeight = INT_NULL;

			inputNumberOfPolyDots = thisData->m_NumberOfPolyDots;

			for (int polyIdx = 0; polyIdx < inputNumberOfPolyDots; ++polyIdx)
			{
				inputPolygonDots[polyIdx] = thisData->m_PolygonDots[polyIdx];
			}
		}

		// HatchStyle에 의한 분류.
		inputIsStyleHatched = thisData->m_IsStyleHatched;
		if (inputIsStyleHatched == true)
		{
			inputHatchStyleNumber = thisData->m_HatchStyleNumber;
		}
		else
		{
			inputHatchStyleNumber = INT_NULL;
		}

		// Color
		inputColor.red = thisData->m_Color.red;
		inputColor.green = thisData->m_Color.green;
		inputColor.blue = thisData->m_Color.blue;

		fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			inputDrawingType, inputDrawingPoint.x, inputDrawingPoint.y, inputDrawingWidth, inputDrawingHeight,
			inputIsStyleHatched, inputHatchStyleNumber, inputColor.red, inputColor.green, inputColor.blue,
			inputNumberOfPolyDots);

		for (int polyIdx = 0; polyIdx < inputNumberOfPolyDots; ++polyIdx)
		{
			POINT dotPoint = inputPolygonDots[polyIdx];
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

	// Data Vector 초기화.
	DrawDataVector.clear();

	char buf[1024];
	// Title 제거.
	fgets(buf, 1024, fp);
	std::string dataLine;

	while (fgets(buf, 1024, fp))
	{
		dataLine.erase();
		dataLine.assign(buf);
		DrawDataSet* loadedData = new DrawDataSet;

		// 타입 로드
		loadedData->m_DrawingType = (DrawingType)strSplit(&dataLine);

		// 포인트랑 너비, 높이 결정.
		if (loadedData->m_DrawingType == POLYGON)
		{
			loadedData->m_DrawingPoint.x = INT_NULL;
			loadedData->m_DrawingPoint.y = INT_NULL;

			loadedData->m_DrawingWidth = INT_NULL;
			loadedData->m_DrawingHeight = INT_NULL;
			
			for (int i = 0; i < 4; ++i)
			{
				strSplit(&dataLine);
			}
		}
		else
		{
			loadedData->m_DrawingPoint.x = strSplit(&dataLine);
			loadedData->m_DrawingPoint.y = strSplit(&dataLine);

			loadedData->m_DrawingWidth = strSplit(&dataLine);
			loadedData->m_DrawingHeight = strSplit(&dataLine);
		}

		// 스타일 로드
		int tempBoolVal = strSplit(&dataLine);
		loadedData->m_IsStyleHatched = to_bool(tempBoolVal);
		if (loadedData->m_IsStyleHatched == true)
		{
			loadedData->m_HatchStyleNumber = strSplit(&dataLine);
		}
		else
		{
			loadedData->m_HatchStyleNumber = INT_NULL;
			strSplit(&dataLine);
		}

		// 컬러 로드
		loadedData->m_Color.red = strSplit(&dataLine);
		loadedData->m_Color.green = strSplit(&dataLine);
		loadedData->m_Color.blue = strSplit(&dataLine);

		// 폴리곤 로드
		loadedData->m_NumberOfPolyDots = strSplit(&dataLine);

		for (int i = 0; i < loadedData->m_NumberOfPolyDots; ++i)
		{
			loadedData->m_PolygonDots[i].x = strSplit(&dataLine);
			loadedData->m_PolygonDots[i].y = strSplit(&dataLine);
		}

		// 벡터에 담음
		DrawDataVector.push_back(loadedData);
	}

	fclose(fp);

	// 화면을 지우고 로드한 데이터를 그려줌
	InvalidateRect(hWnd, NULL, TRUE);


	return PERFORM_WELL;
}

// 문자열 앞에서부터 최초로 만나는 쉼표까지의 문자열을 반환하고 그 뒤까지의 문자열을 잘라주는 함수.
// 쉼표 전까지의 문자열은 int형태로 반환하고, 인자로 받은 문자열은 앞을 잘라준다.
int strSplit(std::string* originStr)
{
	int cutPoint;

	cutPoint = originStr->find_first_of(",");

	std::string* strResult = new std::string(originStr->substr(0, cutPoint));

	*originStr = originStr->substr(cutPoint + 1);

	int ret = ::atoi(strResult->c_str());
	return ret;
}

// 인자로 들어온 int를 bool값으로 반환해주는 함수.
bool to_bool(const int boolVal)
{
	if (boolVal == 0)
	{
		return false;
	}
	return true;
}

#endif