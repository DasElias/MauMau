#include "ShouldDisplayTutorial.h"
#include <memory>

namespace card {
	std::wstring const ShouldDisplayTutorial::DEFAULT_SUBKEY = L"Software\\MauMau 3D\\tutorial";

	ShouldDisplayTutorial::ShouldDisplayTutorial(std::wstring subkey) :
			registryKey(subkey) {
	}
	bool ShouldDisplayTutorial::shouldDisplay(TutorialMessage msg) {
		if(isInShouldntDisplayCache(msg)) {
			return false;
		} 

		std::wstring keyName = getKeyValue(msg);
		bool shouldDisplay = registryKey.getDwordOrDefault(keyName, true);
		if(! shouldDisplay) {
			shouldntDisplayCache.insert(msg);
		}
		return shouldDisplay;
	}
	bool ShouldDisplayTutorial::shouldDisplayAndClear(TutorialMessage msg) {
		bool shouldDisplay = this->shouldDisplay(msg);
		if(shouldDisplay) clear(msg);
		return shouldDisplay;
	}
	void ShouldDisplayTutorial::clear(TutorialMessage msg) {
		std::wstring keyName = getKeyValue(msg);
		registryKey.setDword(keyName, false);
	}
	std::wstring ShouldDisplayTutorial::getKeyValue(TutorialMessage msg) {
		return std::to_wstring(static_cast<unsigned int>(msg));
	}
	bool ShouldDisplayTutorial::isInShouldntDisplayCache(TutorialMessage msg) {
		return std::find(shouldntDisplayCache.begin(), shouldntDisplayCache.end(), msg) != shouldntDisplayCache.end();
	}
}