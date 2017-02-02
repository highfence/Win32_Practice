
// Basic Setting
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ImgScroll");

// 클래스 정의.
class CMyTimer;

class ImgScroller
{
public :
	ImgScroller(HDC hdc, LPCWSTR imgPath, INT scrollPosX, INT scrollPosY, FLOAT runningDeltaTime);
	~ImgScroller();

	void Scroll();

private :
	// 인자없이는 생성 불가.
	ImgScroller();
	HDC m_hdc;
	CMyTimer* m_pTimer;
	CImage* m_pScrollImg;
	FLOAT m_RunningDeltaTime;
	INT m_ScrollPosX;
	INT m_ScrollPosY;
};

class CMyTimer
{
public :
	CMyTimer();
	~CMyTimer() = default;

	void MakeTimerFlag();
	FLOAT GetElapsedTime();

private :
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_TickPerSec;
	BOOL m_StandardFlag;
	
};



// Const Values
const int winWidth = 640;
const int winHeight = 480;



