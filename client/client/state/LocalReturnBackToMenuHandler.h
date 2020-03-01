#pragma once
#include "StateManager.h"
#include "../model/AbstractReturnBackToMenuHandler.h"

namespace card {
	class LocalReturnBackToMenuHandler : public AbstractReturnBackToMenuHandler {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			StateManager& stateManager;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalReturnBackToMenuHandler(StateManager& stateManager);


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void returnBackToMenu() override;
	};
}