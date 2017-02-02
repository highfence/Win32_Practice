#include <Windows.h>
#include <vector>
#include <string>
#include <atlimage.h>
#include "ImgScroll.h"

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

	while (true)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
		}
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

ImgScroller::ImgScroller(HWND hWnd)
	: m_hWnd(hWnd)
{
	m_pTimer = new CMyTimer;
}

ImgScroller::~ImgScroller()
{
	delete m_pTimer;
}

void ImgScroller::LoadData()
{
	BackGroundImg sky1;
	sky1.img.Load(sky1path);
	sky1.scrollX = 0;
	sky1.scrollY = 0;
	sky1.resourceWidth = sky1Width;
	sky1.resourceHeight = sky1Height;
	sky1.scrollSpeed = sky1ScrollSpeed;

	m_ImgVec.push_back(sky1);

	return;
}