#include "Base_include.h"
#include "CInput.h"

#if IS_WIN32
#include "../../QCore/CDInput.h"
#endif

CInput* CInput::instance = nullptr;

void CInput::CreateInput(InputType type)
{
    if (!instance)
        instance = new CInput(type);
}

CInput* CInput::GetInput()
{
    if (!instance)
        GetLogManagerEx()->LogError("Can't get input without creating it!", true);
    return instance;
}

CInput::CInput(InputType type)
{
    switch (type)
    {
    case InputType::DirectInput:
        Input = GetDInput();
        break;
    case InputType::Input:
#pragma message(TODO "fix that")
        Input = GetDInput();
        break;
    default:
#if IS_WIN32
        Input = GetDInput();
#else
        Input = GetLinuxInput();
#endif
        break;
    }
}

void CInput::Update()
{
    Input->Update();
}

bool CInput::IsMouseButtonUp(int button)
{
    return Input->IsMouseButtonUp(button);
}

int* CInput::GetMouseCords()
{
    return Input->GetMousePos();
}
