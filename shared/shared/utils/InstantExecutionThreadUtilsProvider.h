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
			void invokeIn(int delay, std::function<void(void)> callback) override;

	};
}