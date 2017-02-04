#include <Windows.h>
#include <vector>
#include <string>
#include <atlimage.h>
#include "resource2.h"
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

	ImgScroller imgScroller(hWnd);
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
			imgScroller.Scroll();
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
	m_pTimer->MakeTimerFlag();
	LoadData();
}

ImgScroller::~ImgScroller()
{
	delete m_pTimer;
}

void ImgScroller::LoadData()
{
	BackGroundImg* sky1 = new BackGroundImg;

	sky1->imgId = IDB_SKY1;
	sky1->scrollX = 0;
	sky1->scrollY = 0;
	sky1->resourceWidth = sky1Width;
	sky1->resourceHeight = sky1Height;
	sky1->scrollSpeed = sky1ScrollSpeed;

	m_ImgVec.push_back(sky1);

	BackGroundImg* sky2 = new BackGroundImg;

	sky2->imgId = IDB_SKY2;
	sky2->scrollX = 0;
	sky2->scrollY = sky1Height;
	sky2->resourceWidth = sky2Width;
	sky2->resourceHeight = sky2Height;
	sky2->scrollSpeed = sky2ScrollSpeed;
	
	m_ImgVec.push_back(sky2);

	BackGroundImg* sky3 = new BackGroundImg;

	sky3->imgId = IDB_SKY3;
	sky3->scrollX = 0;
	sky3->scrollY = sky1Height + sky2Height;
	sky3->resourceWidth = sky3Width;
	sky3->resourceHeight = sky3Height;
	sky3->scrollSpeed = sky3ScrollSpeed;

	m_ImgVec.push_back(sky3);

	return;
}


void ImgScroller::Scroll()
{
	FLOAT dt = m_pTimer->GetElapsedTime();
	m_pTimer->MakeTimerFlag();
	m_hdc = GetDC(m_hWnd);

	HDC memoryDC = CreateCompatibleDC(m_hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(m_hdc, winWidth, winHeight);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

	// 문제의 지점.
	HDC imgDC = CreateCompatibleDC(m_hdc);
	for (auto i : m_ImgVec)
	{
		i->scrollX -= dt * i->scrollSpeed;
		if (i->scrollX <= -i->resourceWidth)
		{
			i->scrollX += i->resourceWidth;
		}
		HBITMAP hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(i->imgId));
		SelectObject(imgDC, hBitmap);
		BitBlt(memoryDC, i->scrollX, i->scrollY, i->resourceWidth, i->resourceHeight, imgDC, 0, 0, SRCCOPY);
		BitBlt(memoryDC, i->scrollX + i->resourceWidth, i->scrollY, i->resourceWidth, i->resourceHeight, imgDC, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
	}

	BitBlt(m_hdc, 0, 0, winWidth, winHeight, memoryDC, 0, 0, SRCCOPY);

	SelectObject(m_hdc, oldBitmap);
	DeleteObject(memoryBitmap);
	DeleteObject(memoryDC);

	ReleaseDC(m_hWnd, m_hdc);

	return;
}