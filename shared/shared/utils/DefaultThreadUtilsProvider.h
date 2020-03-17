#pragma once
#include "ThreadUtilsProvider.h"
#include <vector>
#include <mutex>

namespace card {
	class DefaultThreadUtilsProvider : public ThreadUtilsProvider {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::pair<const void*, Operation>> pendingOperations;
			std::vector<std::pair<const void*, Operation>> pendingOperations_tempWriteBuffer;
			std::vector<const void*> keysToRemoveInNextUpdate;
			std::vector<const void*> keysToRemoveInNextUpdate_tempWriteBuffer;
			bool useTempWriteBuffer_pendingOperations = false;
			bool useTempWriteBuffer_keysToRemove = false;
			std::mutex invokeInMutex;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update() override;
			void invokeIn(int delay, const void* key, std::function<void(void)> callback) override;
			void removeCallbacks(const void* key) override;

		private:
			void removeCallbacksWithKeysToRemove(std::vector<std::pair<const void*, Operation>>& vector);

	};
}