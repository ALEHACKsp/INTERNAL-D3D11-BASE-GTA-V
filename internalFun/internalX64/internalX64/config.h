#pragma once

class CMainSettings {
private:
	bool debugMode = false;
public:
	void restartSettings(CMainSettings* settings) {
		settings = new CMainSettings();
	}

	void setDebugMode(bool mode) {
		this->debugMode = mode;
	}

	bool getDebugMode() {
		return this->debugMode;
	}
};

extern CMainSettings* mainSettings = new CMainSettings();