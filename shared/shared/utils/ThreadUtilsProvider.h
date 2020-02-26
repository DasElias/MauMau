#pragma once
#include <functional>

namespace card {
	class ThreadUtilsProvider {
		protected:
			struct Operation {
				std::function<void(void)> callback;
				long long beginTimeMs;
			};

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			ThreadUtilsProvider() = default;

		public:
			virtual ~ThreadUtilsProvider() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void update() =0;
			virtual void invokeIn(int delay, const void* key, std::function<void(void)> callback) =0;
			virtual void removeCallbacks(const void* key) =0;
	};
}