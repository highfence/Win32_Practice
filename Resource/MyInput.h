#pragma once
#include <windows.h>

class CMyInput
{
public:
	CMyInput(void){};
	~CMyInput(void){};

	// Ű ���� ����.
	// ���ϰ��� true �ش� Ű�� ���ȴ�.
	//			false �ش� Ű�� �ȴ��ȴ�.
	inline bool GetKey(int KeyCode) const
	{
		return (GetAsyncKeyState(KeyCode) ? 1 : 0);
	}
};
