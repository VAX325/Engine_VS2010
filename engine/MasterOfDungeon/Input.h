#include "windows.h"

#include <dinput.h>

#define DIRECTINPUT_VERSION 0x0800

class DxInput
{
public:
	DxInput();
	~DxInput();

	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void ShutdownInput();
	bool Frame();

	bool IsKeyPressed(int DIK, int DIK_NUMBER);

	bool IsRMouseButtonPressed();
	bool IsLMouseButtonPressed();

	void MouseMove(int* deltaX, int* deltaY);

private:
	bool ReadKeyboard();
	bool ReadMouse();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
};