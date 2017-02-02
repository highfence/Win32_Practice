// Basic Setting
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ImgScroll");

// 클래스 정의.
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

typedef struct BackGroundImg
{
	CImage img;
	INT resourceWidth;
	INT resourceHeight;
	INT scrollX;
	INT scrollY;
	INT scrollSpeed;
};

class ImgScroller
{
public :
	ImgScroller(HWND hWnd);
	~ImgScroller();

	void LoadData();

private :
	// 인자없이 생성 불가.
	ImgScroller();
	
	CMyTimer* m_pTimer;
	HWND m_hWnd;
	std::vector<BackGroundImg> m_ImgVec;
};


// Const Values
const INT winWidth = 640;
const INT winHeight = 480;

LPCTSTR sky1path(L"../Resource/sky1.bmp");
const INT sky1Width = 640;
const INT sky1Height = 124;
const INT sky1ScrollSpeed = 30;

// Functions

