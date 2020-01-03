#pragma once
#include "../network/NetworkErrorHandler.h"
#include "StateManager.h"

namespace card {
	class MainMenuNetworkErrorHandler : public NetworkErrorHandler {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            StateManager& stateManager;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MainMenuNetworkErrorHandler(StateManager& stateManager);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void resetAndShowError(std::string errorMessage, std::string title) override;

	};
}