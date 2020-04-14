#include "ShouldDisplayTutorial.h"
#include "../system/DwordRegistryKey.h"
#include <memory>

namespace card {
	std::wstring const ShouldDisplayTutorial::SUBKEY = L"Software\\MauMau 3D";

	ShouldDisplayTutorial::ShouldDisplayTutorial() :
			registryKey(SUBKEY) {
	}
	bool ShouldDisplayTutorial::shouldDisplay(TutorialMessage msg) {
		std::wstring keyName = getKeyValue(msg);
		return registryKey.getValueOrDefault(keyName, true);
	}
	bool ShouldDisplayTutorial::shouldDisplayAndClear(TutorialMessage msg) {
		bool shouldDisplay = this->shouldDisplay(msg);
		if(shouldDisplay) clear(msg);
		return shouldDisplay;
	}
	void ShouldDisplayTutorial::clear(TutorialMessage msg) {
		std::wstring keyName = getKeyValue(msg);
		registryKey.setValue(keyName, false);
	}
	std::wstring ShouldDisplayTutorial::getKeyValue(TutorialMessage msg) {
		return std::to_wstring(static_cast<unsigned int>(msg));
	}
}