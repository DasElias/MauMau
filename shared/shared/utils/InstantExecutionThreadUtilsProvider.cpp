#include "InstantExecutionThreadUtilsProvider.h"

namespace card {
	void InstantExecutionThreadUtilsProvider::update() {
		// empty method body
	}
	void InstantExecutionThreadUtilsProvider::invokeIn(int delay, const void* key, std::function<void(void)> callback) {
		callback();
	}
	void InstantExecutionThreadUtilsProvider::removeCallbacks(const void* key) {
		// empty method body
	}
}