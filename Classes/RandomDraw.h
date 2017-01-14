#pragma once

const int MAX_POLYGON_DOT = 10;

enum DrawingType
{
	RECTANGLE, CIRCLE, POLYGON
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
	POINT_t FirstDot;
	POINT_t SecondDot;
	RGB_t Color;
	int ColoringType[7];
	POINT_t PolygonDots[MAX_POLYGON_DOT];
	
	// Member Function
};