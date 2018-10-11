// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
#endif
#include <assert.h>

// reference additional headers your program requires here
#include <Renderer.h>

#define GLEW_STATIC
#include <glew.h>
#include <stdio.h>
extern "C" __declspec(dllexport) Light::render::RenderDevice* CreateInterface();