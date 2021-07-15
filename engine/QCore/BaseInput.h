#pragma once
#ifndef BASEINPUT
#define BASEINPUT

class BaseInput
{
public:
	BaseInput();
	virtual ~BaseInput();

	virtual bool Init() { return false; }
	virtual void Release() {}
	virtual bool Update() { return false; }

	virtual bool IsKeyPressed(int key) { return false; }

	virtual int* GetMousePos() { int cords[2] = { 0, 0 }; return cords; };

	virtual bool IsMouseButtonUp(int key) { return false; }
	virtual bool IsMouseButtonDown(int key) { return false; }
};

#endif