#pragma once

namespace card {
	enum class ReturnBackToMenuCause {
		GAME_HAS_ENDED,
		GAME_WAS_ABORTED
	};

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
			virtual void returnBackToMenu(ReturnBackToMenuCause cause) =0;
	};
}