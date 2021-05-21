#pragma once

int __stdcall mainThread() {
	mainSettings->setDebugMode(true);
	internalEngine->attachConsole(mainSettings->getDebugMode());

	hookingMethod->start();

	std::cout << "hooking d3d11\n" << std::endl;

	renderEngine->getD3D11VTables();
	MH_CreateHook((BYTE*)pSwapChainVtable[8], &hookD3D11Present, &phookD3D11Present);
	MH_CreateHook((BYTE*)pDeviceContextVTable[12], &hookD3D11DrawIndexed, &phookD3D11DrawIndexed);
	MH_CreateHook((BYTE*)pDeviceContextVTable[50], &hookD3D11ClearRenderTargetView, &phookD3D11ClearRenderTargetView);

	std::cout << "starting hooks\n" << std::endl;
	hookingMethod->runHooks();

	while ((GetAsyncKeyState('X') & 0x1) == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::cout << "unloading\n" << std::endl;

	renderEngine->stop();
	hookingMethod->stop();
	internalEngine->unattachConsole();
	return 0;
}