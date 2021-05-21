#pragma once

class CHookingMethod {
private:
public:

	static inline auto getVirtual(void* base, unsigned int index) {
		return static_cast<uintptr_t>((*reinterpret_cast<int**>(base))[index]);
	}

	void start() {
		MH_Initialize();
	}

	void runHooks() {
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void stop() {
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
};

extern CHookingMethod* hookingMethod = new CHookingMethod();
