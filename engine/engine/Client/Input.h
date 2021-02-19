#pragma once

#include "windows.h"
#include "CMouse.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class CInput
{
public:
	CInput();
	~CInput();

	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void ShutdownInput();
	bool Frame();

	bool IsKeyPressed(int DIK, int DIK_NUMBER);

	CMouse* GetMouse();

	IDirectInput8* GetDI();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	unsigned char m_keyboardState[256];

	bool ReadKeyboard();

	CMouse* m_pMouse;
};

CInput* GetInputObj();