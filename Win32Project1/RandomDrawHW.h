#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <vector>
#include <string>

const int MAX_POLYGON_DOT = 10;
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int HATCH_STYLE_NUM = 6;
const int DRAW_DATA_MAX_NUM = 256;

enum DrawingType
{
	RECTANGLE, CIRCLE, POLYGON, TYPE_NUM
};

enum RETURN_TYPE
{
	PERFORM_WELL = 0,
	ERROR_FILE_OPEN = 1
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

// Global DrawDataSet Structure;
std::vector<DrawDataSet*> DrawDataVector;

// Fuctions
void Draw(HWND, DrawDataSet*);
int SaveFile();
int LoadFile(HWND);
std::string* strSplit(std::string*);