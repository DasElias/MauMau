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
			std::vector<Operation> pendingOperations;
			std::vector<Operation> tempWriteBuffer;
			bool useTempWriteBuffer = false;
			std::mutex invokeInMutex;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update() override;
			void invokeIn(int delay, std::function<void(void)> callback) override;

	};
}