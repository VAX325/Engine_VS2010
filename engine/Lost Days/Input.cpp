#include "Input.h"

DxInput::DxInput()
{
    m_directInput = 0;
    m_keyboard = 0;
    m_mouse = 0;
}

DxInput::~DxInput()
{
}

bool DxInput::Initialize(HINSTANCE hinstance, HWND hwnd)
{
    HRESULT result;

    // Initialize the main direct input interface.
    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Initialize the direct input interface for the keyboard.
    result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the data format.  In this case since it is a keyboard we can use the predefined data format.
    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result))
    {
        return false;
    }

    
    // Set the cooperative level of the keyboard to not share with other programs.
    result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if(FAILED(result))
    {
        return false;
    }
    
    // Now acquire the keyboard.
    result = m_keyboard->Acquire();
    if (FAILED(result))
    {
        return false;
    }

    // Initialize the direct input interface for the mouse.
    result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the data format for the mouse using the pre-defined mouse data format.
    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(result))
    {
        return false;
    }

    // Acquire the mouse.
    result = m_mouse->Acquire();
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void DxInput::ShutdownInput()
{
    // Release the mouse.
    if (m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = 0;
    }

    // Release the keyboard.
    if (m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }

    // Release the main interface to direct input.
    if (m_directInput)
    {
        m_directInput->Release();
        m_directInput = 0;
    }
}

bool DxInput::ReadKeyboard()
{
    HRESULT result;

    // Read the keyboard device.
    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
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

bool DxInput::ReadMouse()
{
    HRESULT result;


    // Read the mouse device.
    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if (FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool DxInput::Frame()
{
    bool result;


    // Read the current state of the keyboard.
    result = ReadKeyboard();
    if (!result)
    {
        return false;
    }

    // Read the current state of the mouse.
    result = ReadMouse();
    if (!result)
    {
        return false;
    }

    return true;
}

bool DxInput::IsKeyPressed(int DIK, int DIK_NUMBER)
{
    if (m_keyboardState[DIK] & DIK_NUMBER)
    {
        return true;
    }

    return false;
}

void DxInput::MouseMove(int* deltaX, int* deltaY)
{
    *deltaX = m_mouseState.lX;
    *deltaY = m_mouseState.lY;
}

bool DxInput::IsRMouseButtonPressed()
{
    if (m_mouseState.rgbButtons[1] & 0x80)
    {
        return true;
    }
    return false;
}

bool DxInput::IsLMouseButtonPressed()
{
    if (m_mouseState.rgbButtons[0] & 0x80)
    {
        return true;
    }
    return false;
}