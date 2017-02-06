#define IMG_SCROLL
#ifdef IMG_SCROLL

#include <Windows.h>
#include <vector>
#include <string>
#include "resource2.h"
#include "ImgScroll.h"
#include "KeyState.h"

ImgScroller imgScroller;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevhInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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

	imgScroller.SethWnd(hWnd);
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
			imgScroller.GetKeyState();
			imgScroller.MakeAirplaneMove();
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

	case WM_MOUSEMOVE :
		imgScroller.SetAirplanePos(LOWORD(lParam), HIWORD(lParam));
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

ImgScroller::ImgScroller()
	: m_AccTime(0.f)
{
	m_pAirplane = new Airplane;
	m_pTimer = new CMyTimer;
	m_pTimer->MakeTimerFlag();

	LoadData();
}


ImgScroller::~ImgScroller()
{
	delete m_pAirplane;
	delete m_pTimer;
	m_ImgVec.clear();
	std::vector<BackGroundImg*>().swap(m_ImgVec);

	DeleteObject(m_MemoryBitmap);
	DeleteObject(m_MemoryDC);
	DeleteObject(m_ImgDC);
	ReleaseDC(m_hWnd, m_hdc);
}

void ImgScroller::SethWnd(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hdc = GetDC(m_hWnd);	

	m_MemoryDC = CreateCompatibleDC(m_hdc);
	m_MemoryBitmap = CreateCompatibleBitmap(m_hdc, winWidth, winHeight);
	m_OldBitmap = (HBITMAP)SelectObject(m_MemoryDC, m_MemoryBitmap);
	m_ImgDC = CreateCompatibleDC(m_hdc);

	return;
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
	m_AccTime += dt;
	if (m_AccTime > airplaneAniTime)
	{
		m_pAirplane->m_imgVer = (m_pAirplane->m_imgVer + 1) % 2;
		m_AccTime -= airplaneAniTime;
	}

	m_pTimer->MakeTimerFlag();

	BitBltVec(dt);
	m_pAirplane->animate(m_hWnd, m_ImgDC, m_MemoryDC);

	BitBlt(m_hdc, 0, 0, winWidth, winHeight, m_MemoryDC, 0, 0, SRCCOPY);
	SelectObject(m_hdc, m_OldBitmap);

	return;
}

Airplane::Airplane()
	: m_imgId(IDB_AIRPLANE), 
	  m_imgWidth(airplaneWidth),
	  m_imgHeight(airplaneHeight),
	  m_posX(0),
	  m_posY(0),
	  m_imgVer(0)
{}

void ImgScroller::SetAirplanePos(INT x, INT y)
{
	m_pAirplane->m_posX = x;
	m_pAirplane->m_posY = y;
	return;
}

void Airplane::animate(HWND m_hWnd, HDC m_ImgDC, HDC m_MemoryDC)
{
	HBITMAP airplaneBitmap = LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(m_imgId));
	SelectObject(m_ImgDC, airplaneBitmap);

	if (m_imgVer == 0)
	{
		BitBlt(m_MemoryDC, m_posX, m_posY, m_imgWidth, m_imgHeight, m_ImgDC, 0, 0, SRCAND);
		BitBlt(m_MemoryDC, m_posX, m_posY, m_imgWidth, m_imgHeight, m_ImgDC, 0, m_imgHeight, SRCPAINT);
	}
	else
	{
		BitBlt(m_MemoryDC, m_posX, m_posY, m_imgWidth, m_imgHeight, m_ImgDC, m_imgWidth, 0, SRCAND);
		BitBlt(m_MemoryDC, m_posX, m_posY, m_imgWidth, m_imgHeight, m_ImgDC, m_imgWidth, m_imgHeight, SRCPAINT);
	}
	DeleteObject(airplaneBitmap);
	return;
}

void ImgScroller::BitBltVec(FLOAT dt)
{
	for (auto i : m_ImgVec)
	{
		i->scrollX -= dt * i->scrollSpeed;
		if (i->scrollX <= -i->resourceWidth)
		{
			i->scrollX += i->resourceWidth;
		}

		HBITMAP hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(i->imgId));
		SelectObject(m_ImgDC, hBitmap);
		BitBlt(m_MemoryDC, i->scrollX, i->scrollY, i->resourceWidth, i->resourceHeight, m_ImgDC, 0, 0, SRCCOPY);
		BitBlt(m_MemoryDC, i->scrollX + i->resourceWidth, i->scrollY, i->resourceWidth, i->resourceHeight, m_ImgDC, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
	}

	return;
}


void ImgScroller::GetKeyState()
{
	if (GetKeyboardState(m_ByKey))
	{
		for (int i = 0; i < keyboardNumber; ++i)
		{
			// 현재 키입력이 있는 경우.
			if (m_ByKey[i] & 0x80)
			{
				// 그 전 키입력이 없었다면 시작상태로 만들어준다 (PUSHKEY)
				if (!m_OldKey[i])
				{
					m_OldKey[i] = 1;
					m_ByKey[i] |= 0x40;
				}
				// 아니라면 그냥 누르고 있는 중 (HOLDKEY)
			}
			// 키입력이 현재 없는 경우.
			else
			{
				// 그 전 키입력이 없었다면 띄는 상태로 만들어준다. (PULLKEY)
				if (m_OldKey[i])
				{
					m_OldKey[i] = 0;
					m_ByKey[i] = 0x20;
				}
				// 아니라면 그냥 안누르고 있는 상태.
				else
				{
					m_ByKey[i] = 0x10;
				}
			}

		}
	}

	return;
}

void ImgScroller::MakeAirplaneMove()
{
	const INT movePixel = 1;
	if (m_ByKey['W'] & HOLDKEY)
	{
		m_pAirplane->m_posY -= movePixel;
	}
	if (m_ByKey['S'] & HOLDKEY)
	{
		m_pAirplane->m_posY += movePixel;
	}
	if (m_ByKey['A'] & HOLDKEY)
	{
		m_pAirplane->m_posX -= movePixel;
	}
	if (m_ByKey['D'] & HOLDKEY)
	{
		m_pAirplane->m_posX += movePixel;
	}

	return;
}

#endif

