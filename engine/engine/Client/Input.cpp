#include "Input.h"
#include <iostream>
#include <Base_include.h>
#include "ClientPerems.h"

CInput::CInput()
{
	m_directInput = 0;
	m_keyboard = 0;
}

CInput::~CInput()
{
}

bool CInput::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	m_pMouse = new CMouse(GetMainWnd(), false);

	return true;
}

void CInput::ShutdownInput()
{
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool CInput::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CInput::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	m_pMouse->Update();

	return true;
}

bool CInput::IsKeyPressed(int DIK, int DIK_NUMBER)
{
	if (m_keyboardState[DIK] & DIK_NUMBER)
	{
		return true;
	}

	return false;
}

CMouse* CInput::GetMouse()
{
	return m_pMouse;
}

IDirectInput8* CInput::GetDI()
{
	return m_directInput;
}
