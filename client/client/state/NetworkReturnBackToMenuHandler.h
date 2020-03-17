#pragma once
#include "StateManager.h"
#include "../model/AbstractReturnBackToMenuHandler.h"

namespace card {
	class NetworkReturnBackToMenuHandler : public AbstractReturnBackToMenuHandler {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			StateManager& stateManager;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			NetworkReturnBackToMenuHandler(StateManager& stateManager);


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void returnBackToMenu(ReturnBackToMenuCause cause) override;
	};
}