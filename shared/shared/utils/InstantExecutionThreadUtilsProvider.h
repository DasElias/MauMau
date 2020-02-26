#pragma once
#include "ThreadUtilsProvider.h"
#include <vector>

namespace card {
	class InstantExecutionThreadUtilsProvider : public ThreadUtilsProvider {
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update() override;
			void invokeIn(int delay, const void* key, std::function<void(void)> callback) override;
			void removeCallbacks(const void* key) override;

	};
}