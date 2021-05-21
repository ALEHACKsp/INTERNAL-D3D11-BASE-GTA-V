#define _CRT_SECURE_NO_WARNINGS
#include "includes.h"

bool __stdcall DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	internalEngine->setLocalModule(hModule);

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainThread, NULL, NULL, NULL);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {

	}

	return TRUE;
}
