#pragma once

#include <dinput.h>

struct iMouseReceiver
{
	virtual void MouseMoved(int dx, int dy) {};
	virtual void MouseButtonUp(int button) { };
	virtual void MouseButtonDown(int button) { };
};

class CMouse
{
public:

	CMouse(HWND hWnd, bool bExclusive);
	~CMouse();

	void SetReceiver(iMouseReceiver* pTarget);

	void Update();

	void Acquire();
	void UnAcquire();

	DIMOUSESTATE GetLastState() { return m_lastState; }

private:

	LPDIRECTINPUTDEVICE8	m_pDevice;

	DIMOUSESTATE			m_lastState;

	iMouseReceiver* m_pTarget;
};