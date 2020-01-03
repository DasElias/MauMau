#include "DefaultThreadUtilsProvider.h"
#include "../utils/TimeUtils.h"
#include <algorithm>
namespace card {
	void DefaultThreadUtilsProvider::update() {
		long long ms = getMilliseconds();

		useTempWriteBuffer = true;
		pendingOperations.erase(std::remove_if(pendingOperations.begin(), pendingOperations.end(), [ms](const Operation& o) {
			if(ms >= o.beginTimeMs) {
				o.callback();
				return true;
			}
			return false;
		}), pendingOperations.end());
		useTempWriteBuffer = false;

		for(auto& o : tempWriteBuffer) {
			pendingOperations.push_back(o);
		}
		tempWriteBuffer.clear();
	}
	void DefaultThreadUtilsProvider::invokeIn(int delay, std::function<void(void)> callback) {
		std::lock_guard<std::mutex> lockGuard(invokeInMutex);

		Operation op = {callback, getMilliseconds() + delay};
		if(useTempWriteBuffer) {
			tempWriteBuffer.push_back(op);
		} else {
			pendingOperations.push_back(op);
		}
	}
}