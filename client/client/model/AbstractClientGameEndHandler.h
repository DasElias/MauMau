#pragma once

namespace card {
	class AbstractClientGameEndHandler {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			AbstractClientGameEndHandler() = default;

		public:
			virtual ~AbstractClientGameEndHandler() = default;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void onGameEnd() =0;
	};
}