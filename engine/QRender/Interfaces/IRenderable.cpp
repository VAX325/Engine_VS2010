#include <Base_include.h>
#include "IRenderable.h"

#include "../DirectX/DX11/IRenderable11.h"
#include "../DirectX/DX9/IRenderable9.h"

extern int GetCurrentRenderSpec();

IRenderable::IRenderable()
{
//	int result = GetCurrentRenderSpec();
//#if IS_WIN32
//#if IS_WIN10
//	if (result == 12)
//		Render12 = new IRenderable12();
//#endif
//	if (result == 11)
//		Render11 = new IRenderable11(, Sprite, );
//
//	if (result == 9)
//		Render9 = new IRenderable9();
//
//	GetLogManagerEx()->LogError("Render create error!", true);
//#endif
#if IS_WIN10
	Render12 = nullptr;
#endif
	Render11 = nullptr;
	Render9 = nullptr;

	//X = 0;
	//Y = 0;
}

IRenderable::~IRenderable()
{
#if IS_WIN32
#if IS_WIN10
	if (Render12)
		Render12->~IRenderable12();
#endif
	if (Render11)
		Render11->~IRenderable11();

	if (Render9)
		Render9->~IRenderable9();
#endif
}

void IRenderable::SetX(float x)
{
	//X = x;
	if (Render9)
		Render9->SetX(x);
}

void IRenderable::SetY(float y)
{
	//Y = y;
	if(Render9)
		Render9->SetY(y);
}

void IRenderable::SetW(float w)
{
	if (Render9)
		Render9->SetW(w);
}

void IRenderable::SetH(float h)
{
	if (Render9)
		Render9->SetH(h);
}
