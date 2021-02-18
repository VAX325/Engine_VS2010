#include "Base_include.h"
#include "ClientPerems.h"
#include "Input.h"
#include "CMouse.h"

CMouse::CMouse(HWND hWnd, bool bExclusive)
{
	m_pTarget = NULL;

	HRESULT hr;

	hr = GetInputObj()->GetDI()->CreateDevice(GUID_SysMouse, &m_pDevice, NULL);

	if (S_OK != hr)
	{
		GetLogObjCl()->LogError((char*)"Can't create mouse!", true);
	}

	hr = m_pDevice->SetDataFormat(&c_dfDIMouse);

	if (S_OK != hr)
	{
		GetLogObjCl()->LogError((char*)"Can't set mouse format!", true);
	}

	if (bExclusive)
	{
		hr = m_pDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		if (S_OK != hr)
		{
			GetLogObjCl()->LogError((char*)"Can't change cooperative level!", true);
		}
	}
	else
	{
		hr = m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		
		if (S_OK != hr)
		{
			GetLogObjCl()->LogError((char*)"Can't change cooperative level!", true);
		}
	}

	m_lastState.lX = 0;
	m_lastState.lY = 0;
	m_lastState.lZ = 0;
	m_lastState.rgbButtons[0] = 0;
	m_lastState.rgbButtons[1] = 0;
	m_lastState.rgbButtons[2] = 0;
	m_lastState.rgbButtons[3] = 0;
}

CMouse::~CMouse()
{
	if (m_pDevice)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

void CMouse::SetReceiver(iMouseReceiver* pTarget)
{
	m_pTarget = pTarget;
}

void CMouse::Update()
{
	DIMOUSESTATE currState;
	HRESULT  hr;

	hr = m_pDevice->Poll();
	hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&currState);

	if (FAILED(hr))
	{
		hr = m_pDevice->Acquire();
		if (S_OK != hr)
		{
			return;
		}
		hr = m_pDevice->Poll();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&currState);
		if (S_OK != hr)
		{
			return;
		}
	}
	if (m_pTarget)
	{
		int dx = currState.lX;
		int dy = currState.lY;
		if (dx || dy)
		{
			m_pTarget->MouseMoved(dx, dy);
		}
		if (currState.rgbButtons[0] & 0x80)
		{
			m_pTarget->MouseButtonDown(0);
		}
		if (currState.rgbButtons[1] & 0x80)
		{
			m_pTarget->MouseButtonDown(1);
		}
		if (currState.rgbButtons[2] & 0x80)
		{
			m_pTarget->MouseButtonDown(2);
		}
		if (!(currState.rgbButtons[0] & 0x80) && (m_lastState.rgbButtons[0] & 0x80))
		{
			m_pTarget->MouseButtonUp(0);
		}
		if (!(currState.rgbButtons[1] & 0x80) && (m_lastState.rgbButtons[1] & 0x80))
		{
			m_pTarget->MouseButtonUp(1);
		}
		if (!(currState.rgbButtons[2] & 0x80) && (m_lastState.rgbButtons[2] & 0x80))
		{
			m_pTarget->MouseButtonUp(2);
		}
	}
	m_lastState = currState;
}

void CMouse::Acquire()
{
	HRESULT hr;

	hr = m_pDevice->Acquire();

	if (S_OK != hr)
	{
		GetLogObjCl()->LogError((char*)"Can't acquire mouse!", true);
	}
}

void CMouse::UnAcquire()
{
	HRESULT hr;

	hr = m_pDevice->Unacquire();

	if (S_OK != hr)
	{
		GetLogObjCl()->LogError((char*)"Can't unacquire mouse!", true);
	}
}