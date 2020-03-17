#include "DefaultThreadUtilsProvider.h"
#include "../utils/TimeUtils.h"
#include <algorithm>

namespace card {
	void DefaultThreadUtilsProvider::update() {
		long long ms = getMilliseconds();

		useTempWriteBuffer_keysToRemove = true;
		removeCallbacksWithKeysToRemove(pendingOperations);
		keysToRemoveInNextUpdate.clear();
		useTempWriteBuffer_keysToRemove = false;

		for(auto& o : keysToRemoveInNextUpdate_tempWriteBuffer) {
			keysToRemoveInNextUpdate.push_back(o);
		}
		keysToRemoveInNextUpdate_tempWriteBuffer.clear();


		useTempWriteBuffer_pendingOperations = true;
		pendingOperations.erase(std::remove_if(pendingOperations.begin(), pendingOperations.end(), [this, ms](const std::pair<const void*, Operation>& operationWrapper) {
			const auto& o = operationWrapper.second;
			const void* removeKey = operationWrapper.first;
			bool wasCallbackAlreadyRemovedThisTurn = std::find(keysToRemoveInNextUpdate.begin(), keysToRemoveInNextUpdate.end(), removeKey) != keysToRemoveInNextUpdate.end();
			if(wasCallbackAlreadyRemovedThisTurn) return false;

			if(ms >= o.beginTimeMs) {
				o.callback();
				return true;
			}
			return false;
		}), pendingOperations.end());
		useTempWriteBuffer_pendingOperations = false;

		for(auto& o : pendingOperations_tempWriteBuffer) {
			pendingOperations.push_back(o);
		}
		pendingOperations_tempWriteBuffer.clear();
	}
	void DefaultThreadUtilsProvider::invokeIn(int delay, const void* key, std::function<void(void)> callback) {
		Operation op = {callback, getMilliseconds() + delay};
		if(useTempWriteBuffer_pendingOperations) {
			pendingOperations_tempWriteBuffer.push_back(std::make_pair(key, op));
		} else {
			pendingOperations.push_back(std::make_pair(key, op));
		}
	}

	void DefaultThreadUtilsProvider::removeCallbacks(const void* key) {
		if(useTempWriteBuffer_keysToRemove) {
			keysToRemoveInNextUpdate_tempWriteBuffer.push_back(key);
		} else {
			keysToRemoveInNextUpdate.push_back(key);
		}		
	}

	void DefaultThreadUtilsProvider::removeCallbacksWithKeysToRemove(std::vector<std::pair<const void*, Operation>>& vector) {
		vector.erase(std::remove_if(vector.begin(), vector.end(), [this](const std::pair<const void*, Operation>& operationWrapper) {
			const void* operationKey = operationWrapper.first;
			return std::find(keysToRemoveInNextUpdate.begin(), keysToRemoveInNextUpdate.end(), operationKey) != std::end(keysToRemoveInNextUpdate);
		}), vector.end());
	}
	
	
}