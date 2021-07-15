#include "Base_include.h"
#include "CDInput.h"

CDInput::CDInput()
{
	DirectInput = nullptr;
	m_keyboard = nullptr;

	if (!Init()) 
	{
		GetLogManagerEx()->LogError("Can't init the DirevtInput", true);
	}
}

CDInput::~CDInput()
{
}

bool CDInput::InitKeyboard(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = DirectInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
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

	return true;
}

bool CDInput::InitMouse(HWND hwnd, bool bExclusive)
{
	HRESULT hr;

	hr = DirectInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (S_OK != hr)
	{
		GetLogManagerEx()->LogError("Can't create mouse!", true);
	}

	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (S_OK != hr)
	{
		GetLogManagerEx()->LogError("Can't set mouse format!", true);
	}

	if (bExclusive)
	{
		hr = m_mouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		if (S_OK != hr)
		{
			GetLogManagerEx()->LogError("Can't change cooperative level!", true);
		}
	}
	else
	{
		hr = m_mouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		if (S_OK != hr)
		{
			GetLogManagerEx()->LogError("Can't change cooperative level!", true);
		}
	}

	lastMouseState.lX = 0;
	lastMouseState.lY = 0;
	lastMouseState.lZ = 0;
	lastMouseState.rgbButtons[0] = 0;
	lastMouseState.rgbButtons[1] = 0;
	lastMouseState.rgbButtons[2] = 0;
	lastMouseState.rgbButtons[3] = 0;

	return true;
}

bool CDInput::Init()
{
	HINSTANCE hinstance = GetModuleHandle(NULL);
	hwnd = GetForegroundWindow();

	if (!InitKeyboard(hinstance, hwnd)) 
	{
		GetLogManagerEx()->LogError("Can't init the keyboard! InputMode: DirectInput", true);
	}

	if (!InitMouse(hwnd))
	{
		GetLogManagerEx()->LogError("Can't init the keyboard! InputMode: DirectInput", true);
	}

	return true;
}

void CDInput::Release()
{
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	if (DirectInput)
	{
		DirectInput->Release();
		DirectInput = nullptr;
	}
}

bool CDInput::Update()
{
	ReadKeyboard();
	ReadMouse();

	return true;
}

bool CDInput::IsKeyPressed(int DIK)
{
	if (m_keyboardState[DIK] & 0x80)
	{
		return true;
	}
	return false;
}

bool CDInput::ReadKeyboard()
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

inline bool CDInput::ReadMouse()
{
	isRightPressed = false;
	isLeftPressed = false;

	HRESULT hr;
	DIMOUSESTATE currState;

	hr = m_mouse->Poll();
	hr = m_mouse->GetDeviceState(sizeof(currState), (LPVOID)&currState);

	if (FAILED(hr))
	{
		hr = m_mouse->Acquire();
		if (S_OK != hr)
		{
			return false;
		}
		hr = m_mouse->Poll();
		hr = m_mouse->GetDeviceState(sizeof(currState), (LPVOID)&currState);
		if (S_OK != hr)
		{
			return false;
		}
	}

	int dx = currState.lX;
	int dy = currState.lY;
	if (dx || dy)
	{
		//m_pTarget->MouseMoved(dx, dy);
	}
	if (currState.rgbButtons[0] & 0x80)
	{
		//m_pTarget->MouseButtonDown(0);
		isLeftPressed = true;
	}
	if (currState.rgbButtons[1] & 0x80)
	{
		//m_pTarget->MouseButtonDown(1);
		isRightPressed = true;
	}
	if (currState.rgbButtons[2] & 0x80)
	{
		//m_pTarget->MouseButtonDown(2);
	}
	if (!(currState.rgbButtons[0] & 0x80) && (lastMouseState.rgbButtons[0] & 0x80))
	{
		//m_pTarget->MouseButtonUp(0);
	}
	if (!(currState.rgbButtons[1] & 0x80) && (lastMouseState.rgbButtons[1] & 0x80))
	{
		//m_pTarget->MouseButtonUp(1);
	}
	if (!(currState.rgbButtons[2] & 0x80) && (lastMouseState.rgbButtons[2] & 0x80))
	{
		//m_pTarget->MouseButtonUp(2);
	}

	lastMouseState = currState;

	return true;
}

bool CDInput::IsMouseButtonUp(int key)
{
	switch (key)
	{
	case 0:
		return isLeftPressed;
		break;
	case 1:
		return isRightPressed;
		break;
	default:
		break;
	}
	return false;
}

bool CDInput::IsMouseButtonDown(int key)
{
	return false;
}

int* CDInput::GetMousePos()
{
	POINT p = POINT();
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);

	int pos[2] = { p.x, p.y };
	return pos;
}

//Singleton Like
CDInput* input;

QCORE_PUBLIC BaseInput* GetDInput()
{
	if (!input)
		input = new CDInput();
	return input;
}
