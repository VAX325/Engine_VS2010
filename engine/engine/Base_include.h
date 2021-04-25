#pragma once
#ifndef _LEVEL_EDITOR
#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#define TODO __FILE__ "(" $Line ") : TODO: "

//Use like: #if IS_WIN32
#define IS_WIN32 PLATFORM == PLATFORM_WINDOWS

//Use like: #if IS_MAC
#define IS_MAC PLATFORM == PLATFORM_MAC

//Use like: #if IS_UNIX
#define IS_UNIX PLATFORM == PLATFORM_UNIX

#define STANDART_PORT 27063

#include "../UsefullMacro.h"
#include "../QCore/Log/CLogManager.h"

#endif
#endif