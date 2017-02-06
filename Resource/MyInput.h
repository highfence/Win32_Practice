#pragma once
#include <windows.h>

class CMyInput
{
public:
	CMyInput(void){};
	~CMyInput(void){};

	// 키 값을 전달.
	// 리턴값이 true 해당 키가 눌렸다.
	//			false 해당 키가 안눌렸다.
	inline bool GetKey(int KeyCode) const
	{
		return (GetAsyncKeyState(KeyCode) ? 1 : 0);
	}
};
