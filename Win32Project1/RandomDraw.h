#pragma once

const int MAX_POLYGON_DOT = 10;
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

enum DrawingType
{
	RECTANGLE, CIRCLE, POLYGON, TYPE_NUM
};

typedef struct point
{
	unsigned int x;
	unsigned int y;

} POINT_t;

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
	RandomDraw();
	~RandomDraw() = default;

	// Member variable
	DrawingType m_DrawingType;
	POINT_t m_FirstDot;
	POINT_t m_SecondDot;
	RGB_t m_Color;
	POINT_t m_PolygonDots[MAX_POLYGON_DOT];
	
	// Member Function
	bool InitDataSetting();
	void Draw();
};