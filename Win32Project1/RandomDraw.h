#pragma once
#include <Windows.h>

const int MAX_POLYGON_DOT = 10;
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int HATCH_STYLE_NUM = 6;

enum DrawingType
{
	RECTANGLE, CIRCLE, POLYGON, TYPE_NUM
};

typedef struct rgb
{
	unsigned int red;
	unsigned int green;
	unsigned int blue;

} RGB_t;

class RandomDraw
{
public :

	// Create and destroy
	RandomDraw(HWND);
	~RandomDraw() = default;

	// Member variable
	DrawingType m_DrawingType;
	POINT m_FirstDot;
	POINT m_SecondDot;
	bool m_IsStyleHatched;
	int m_HatchStyleNumber;
	RGB_t m_Color;
	POINT m_PolygonDots[MAX_POLYGON_DOT];
	int m_NumberOfPolyDots	;

	// Create Parameter;
	HWND m_Hwnd;
	PAINTSTRUCT m_Ps;
	
	// Member Function
	bool InitDataSetting(HWND);
	void Draw();
};