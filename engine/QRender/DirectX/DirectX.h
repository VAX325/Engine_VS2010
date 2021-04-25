#pragma once
#ifndef DIRECTX_H
#define DIRECTX_H

#include "../BaseManager.h"

#ifdef IS_WIN10
#include "DX12/directx12.h"
#endif
#include "DX11/directx11.h"
#include "DX9/directx9.h"

int InitDirectX(Function PreUpdate);

#endif