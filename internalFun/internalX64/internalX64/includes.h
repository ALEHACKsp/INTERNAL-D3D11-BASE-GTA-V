#include <thread>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <consoleapi.h>

#include <MinHook.h>
#pragma comment(lib, "./minhook/bin/MinHook-Debug.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d11.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d11.lib")

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#define isValidPtr(pointer) ((uintptr_t)pointer > 0x1000 && (uintptr_t)pointer < 0x7fffffffffff)

#include "vec3.h"
#include "gameSdk.h"

#include "playerEsp.h"

#include "internalEngine.h"
#include "hooks.h"
#include "config.h"

#include "renderEngine.h"

#include "main.h"