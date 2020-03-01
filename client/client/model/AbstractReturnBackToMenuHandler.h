#pragma once

namespace card {
	class AbstractReturnBackToMenuHandler {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			AbstractReturnBackToMenuHandler() = default;

		public:
			virtual ~AbstractReturnBackToMenuHandler() = default;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void returnBackToMenu() =0;
	};
}