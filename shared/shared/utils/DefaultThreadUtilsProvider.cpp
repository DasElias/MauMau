#include "DefaultThreadUtilsProvider.h"
#include "../utils/TimeUtils.h"
#include <algorithm>
namespace card {
	void DefaultThreadUtilsProvider::update() {
		long long ms = getMilliseconds();

		useTempWriteBuffer = true;
		pendingOperations.erase(std::remove_if(pendingOperations.begin(), pendingOperations.end(), [ms](const std::pair<const void*, Operation>& operationWrapper) {
			const auto& o = operationWrapper.second;
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
	void DefaultThreadUtilsProvider::invokeIn(int delay, const void* key, std::function<void(void)> callback) {
		std::lock_guard<std::mutex> lockGuard(invokeInMutex);

		Operation op = {callback, getMilliseconds() + delay};
		if(useTempWriteBuffer) {
			tempWriteBuffer.push_back(std::make_pair(key, op));
		} else {
			pendingOperations.push_back(std::make_pair(key, op));
		}
	}

	void DefaultThreadUtilsProvider::removeCallbacks(const void* key) {
		removeCallbackImpl(pendingOperations, key);
		removeCallbackImpl(tempWriteBuffer, key);
	}

	void DefaultThreadUtilsProvider::removeCallbackImpl(std::vector<std::pair<const void*, Operation>>& vector, const void* key) {
		vector.erase(std::remove_if(vector.begin(), vector.end(), [key](const std::pair<const void*, Operation>& operationWrapper) {
			const void* operationKey = operationWrapper.first;
			return key == operationKey;
		}), vector.end());
	}
	
	
}