#pragma once
#ifndef CINPUT_H
#define CINPUT_H

#include "../../QCore/BaseInput.h"

enum class InputType
{
#if IS_WIN32
	DirectInput,
#endif
	Input,
};

class CInput
{
public:
	static void CreateInput(InputType type = InputType::Input);
	static CInput* GetInput();

	void Update();

	bool IsMouseButtonUp(int button);
	int* GetMouseCords();

private:
	static CInput* instance;

	CInput(InputType type);
	CInput(const CInput&);
	CInput& operator=(CInput&);

	BaseInput* Input;
};

#endif