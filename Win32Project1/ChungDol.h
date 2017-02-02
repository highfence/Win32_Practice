#pragma once

// Basic Setting
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ChungDol");
LPCTSTR collisionResource = TEXT("../Resource/Colllision.bmp");

// Const Values
namespace RectValues
{
	const int rectX1 = 300;
	const int rectY1 = 300;
	const int rectX2 = 600;
	const int rectY2 = 600;
	TCHAR* infoStr = TEXT("충돌 박스입니다.");
}

// Static Values
static int mouseX = 0;
static int mouseY = 0;
static bool isMouseOnRect = false;


// Functions
void PaintProc(HWND, HDC, PAINTSTRUCT);
void DrawRect(HWND, HDC, HBRUSH, HBRUSH);
void DrawImg(HDC);
bool CheckMouseIsOnRect();