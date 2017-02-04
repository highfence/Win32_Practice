// Basic Setting
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ImgScroll");

// Ŭ���� ����.
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
	WORD imgId;
	INT resourceWidth;
	INT resourceHeight;
	FLOAT scrollX;
	FLOAT scrollY;
	INT scrollSpeed;
};

class ImgScroller
{
public :
	ImgScroller(HWND hWnd);
	~ImgScroller();

	void LoadData();
	void Scroll();

private :
	// ���ھ��� ���� �Ұ�.
	ImgScroller();
	
	CMyTimer* m_pTimer;
	HWND m_hWnd;
	HDC m_hdc;
	std::vector<BackGroundImg*> m_ImgVec;
};

// Const Values
const INT winWidth = 640;
const INT winHeight = 480;

const INT sky1Width = 640;
const INT sky1Height = 124;
const INT sky1ScrollSpeed = 192;

const INT sky2Width = 640;
const INT sky2Height = 214;
const INT sky2ScrollSpeed = 128;

const INT sky3Width = 640;
const INT sky3Height = 142;
const INT sky3ScrollSpeed = 64;