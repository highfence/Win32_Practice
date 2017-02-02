#include <Windows.h>
#include "ImgScroll.h"
#include <atlimage.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevhInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
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
		CW_USEDEFAULT, CW_USEDEFAULT, winWidth, winHeight,
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
	switch (iMessage)
	{
	case WM_CREATE :

		return 0;

	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}


ImgScroller::ImgScroller(HDC hdc, LPCWSTR imgPath, INT scrollPosX, INT scrollPosY, FLOAT runningDeltaTime)
	: m_hdc(hdc), m_ScrollPosX(scrollPosX), m_ScrollPosY(scrollPosY), m_RunningDeltaTime(runningDeltaTime)
{
	m_pTimer = new CMyTimer;
	m_pScrollImg->Load(imgPath);
	m_pTimer->MakeTimerFlag();
	return;
}

ImgScroller::~ImgScroller()
{
	delete m_pTimer;
	delete m_pScrollImg;
	return;
}

void ImgScroller::Scroll()
{
	FLOAT dt = m_pTimer->GetElapsedTime();

	if (dt == -1.f)
	{
		OutputDebugString(L"타이머 고장.");
		return;
	}



	// TODO :: 여기서 부터
	m_pTimer->MakeTimerFlag();

	return;
}

CMyTimer::CMyTimer()
	: m_StandardFlag(false)
{
	if (!QueryPerformanceCounter(&m_CurTime))
	{
		OutputDebugString(L"QueryPerformanceCounter 함수 미지원! \n");
	}
	if (!QueryPerformanceCounter(&m_LastTime))
	{
		OutputDebugString(L"QueryPerformanceCounter 함수 미지원! \n");
	}
	if (!QueryPerformanceFrequency(&m_TickPerSec))
	{
		OutputDebugString(L"QueryPerformanceFrequency 함수 미지원! \n");
		exit(0);
	}

	return;
}

void CMyTimer::MakeTimerFlag()
{
	QueryPerformanceCounter(&m_LastTime);
	m_StandardFlag = true;
	return;
}

FLOAT CMyTimer::GetElapsedTime()
{
	if (m_StandardFlag == false)
	{
		return -1.f;
	}
	
	QueryPerformanceCounter(&m_CurTime);
	FLOAT elapsedSeconds = ((FLOAT)m_CurTime.QuadPart - (FLOAT)m_LastTime.QuadPart) / (FLOAT)m_TickPerSec.QuadPart;
	m_StandardFlag = false;

	return elapsedSeconds;
}