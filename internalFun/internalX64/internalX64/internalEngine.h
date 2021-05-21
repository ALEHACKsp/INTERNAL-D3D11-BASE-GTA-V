#pragma once 

class CInternalEngine {
private:
	bool isCosoleLive = false;
	HMODULE localModule = nullptr;
public:
	void setLocalModule(HMODULE foundModule) {
		this->localModule = foundModule;
	}

	HMODULE getLocalModule() {
		return this->localModule;
	}

	void attachConsole(bool spawnCondition) {
		if (!spawnCondition)
			return;

		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		this->isCosoleLive = true;
	}

	void unattachConsole() {
		if (!isCosoleLive)
			return;

		FreeConsole();
		FreeLibraryAndExitThread(this->localModule, 0);
	}
};

extern CInternalEngine* internalEngine = new CInternalEngine();