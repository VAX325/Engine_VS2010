#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#pragma comment(lib,"winmm.lib")
#if IS_WIN32
#pragma comment(lib, "dinput8.lib")
#endif

#include "ClientPerems.h"

#include <iostream>
#include <vector>
#include <fstream>

int ClientMain(int argc, char* argv[]);

#endif