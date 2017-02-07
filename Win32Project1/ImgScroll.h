// Basic Setting
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ImgScroll");

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

const INT airplaneWidth = 50;
const INT airplaneHeight = 15;
const INT airplaneAniTime = 0.5f;

const INT keyboardNumber = 256;

// 클래스 정의.
class CMyTimer
{
public :
	CMyTimer();
	~CMyTimer() = default;

	void			MakeTimerFlag();
	FLOAT			GetElapsedTime();

private :
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_TickPerSec;
	BOOL			m_StandardFlag;
	
};

typedef struct BackGroundImg
{
	WORD			imgId;
	INT				resourceWidth;
	INT				resourceHeight;
	FLOAT			scrollX;
	FLOAT			scrollY;
	INT				scrollSpeed;
};

class Airplane
{
public :
	Airplane();
	~Airplane() = default;

	void			animate(HWND, HDC, HDC);

	WORD			m_imgId;
	INT				m_imgWidth;
	INT				m_imgHeight;
	INT				m_posX;
	INT				m_posY;
	INT				m_imgVer;
	BOOL			m_IsAirplaneFront;
};

class ImgScroller
{
public :
	ImgScroller();
	~ImgScroller();

	void			LoadData();
	void			Scroll();
	void			SethWnd(HWND);
	void			SetAirplanePos(INT, INT);
	void			BitBltVec(FLOAT);
	void			GetKeyState();
	void			MakeAirplaneMove();

private :
	
	CMyTimer*		m_pTimer;
	Airplane*		m_pAirplane;
	HWND			m_hWnd;
	HDC				m_hdc;
	HDC				m_MemoryDC;
	HDC				m_ImgDC;
	HBITMAP			m_MemoryBitmap;
	HBITMAP			m_OldBitmap;
	FLOAT			m_AccTime;
	BYTE			m_OldKey[keyboardNumber] = { 0 };
	BYTE			m_ByKey[keyboardNumber];

	std::vector<BackGroundImg*> m_ImgVec;
};


