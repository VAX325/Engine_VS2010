#pragma once
#ifndef CDINPUT
#define CDINPUT

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>

#include "BaseInput.h"

class CDInput : public BaseInput
{
public:
	CDInput();
	virtual ~CDInput();

	virtual bool Init();
	virtual void Release();
	virtual bool Update();

	virtual bool IsKeyPressed(int key);

	virtual bool IsMouseButtonUp(int key);
	virtual bool IsMouseButtonDown(int key);

	virtual int* GetMousePos();

private:
	IDirectInput8* DirectInput;

	IDirectInputDevice8* m_keyboard;
	unsigned char m_keyboardState[256];
	inline bool InitKeyboard(HINSTANCE hinstance, HWND hwnd);
	inline bool ReadKeyboard();

	IDirectInputDevice8* m_mouse;
	inline bool InitMouse(HWND hwnd, bool bExclusive = false);
	inline bool ReadMouse();
	DIMOUSESTATE lastMouseState;

	bool isLeftPressed;
	bool isRightPressed;

	HWND hwnd;
};

QCORE_PUBLIC BaseInput* GetDInput();

#endif