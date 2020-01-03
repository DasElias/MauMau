#include "InstantExecutionThreadUtilsProvider.h"

namespace card {
	void InstantExecutionThreadUtilsProvider::update() {
		// empty method body
	}
	void InstantExecutionThreadUtilsProvider::invokeIn(int delay, std::function<void(void)> callback) {
		callback();
	}
}