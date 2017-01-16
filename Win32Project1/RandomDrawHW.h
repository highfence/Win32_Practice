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

class DrawDataSet
{
public :

	// Create and destroy
	DrawDataSet();
	~DrawDataSet() = default;

	// Member Variable
	DrawingType m_DrawingType;
	POINT m_DrawingPoint;
	int m_DrawingWidth;
	int m_DrawingHeight;

	bool m_IsStyleHatched;
	int m_HatchStyleNumber;
	RGB_t m_Color;

	int m_NumberOfPolyDots;
	POINT m_PolygonDots[MAX_POLYGON_DOT];

};


// Fuctions
void Draw(HWND, DrawDataSet*);