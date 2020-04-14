#pragma once

#include <functional>

namespace card {
	template<class W>
	class FunctionWrapper {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int identity;
			std::function<void(W&)> function;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			FunctionWrapper(std::function<void(W&)> function) :
				function(function) {
				static int id = 0;
				identity = id++;
			}

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			inline bool operator==(const FunctionWrapper<W>& other) const {
				return identity == other.identity;
			}
			inline std::function<void(W&)> getFunction() const {
				return function;
			}
			inline void operator()(W& event) {
				function(event);
			};


	};
}


