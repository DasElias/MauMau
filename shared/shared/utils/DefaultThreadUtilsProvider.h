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
			std::vector<std::pair<const void*, Operation>> tempWriteBuffer;
			bool useTempWriteBuffer = false;
			std::mutex invokeInMutex;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update() override;
			void invokeIn(int delay, const void* key, std::function<void(void)> callback) override;
			void removeCallbacks(const void* key) override;

		private:
			void removeCallbackImpl(std::vector<std::pair<const void*, Operation>>& vector, const void* key);

	};
}