#include <Base_include.h>
#include "DirectX.h"

#pragma warning( push )
#pragma warning( disable : 4005)
#include "DX9/directx9.h"
#include "DX11/directx11.h"
#if IS_WIN10
#include "DX12/directx12.h"
#endif
#pragma warning( pop ) 

#include "../Interfaces/QRender.h"

int InitDirectX(Function PreUpdate)
{
#if IS_WIN10
	bool result = InitDirectX12(PreUpdate);

	if (!result)
		result = InitDirectX11(PreUpdate);
	else
		return 12;
#else
	bool result = InitDirectX11(PreUpdate);
#endif

	if (!result)
		result = InitDirectX9(PreUpdate);
	else
		return 11;

	if (!result)
	{
		GetLogManagerEx()->LogError("Can't init the DirectX", true);
	}
	else
	{
		return 9;
	}

	return 0;
}

QRENDER_PUBLIC GraphicsManager* GetGraphicsManager()
{
	GraphicsManager* manager = dynamic_cast<GraphicsManager*>(GetDX11Manager());
	if (!manager)
		manager = dynamic_cast<GraphicsManager*>(GetDX9Manager());
	return manager;
}