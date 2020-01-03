#pragma once
#include <typeinfo>
#include <unordered_map>
#include <memory>

#include "../model/AbstractGameFacade.h"

namespace card {
	class State;

	class StateManager {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::unordered_map<std::string, State*> allStates;
			State* p_currentState = nullptr;
			std::string currentStateName = "";
			std::shared_ptr<AbstractGameFacade> game;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			StateManager();
			StateManager(const StateManager&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getCurrentStateName() const;

			void addState(std::string stateType, State* state);
			void changeState(std::string stateType);
			void updateAndRender(float delta);

			void setGameFacade(std::shared_ptr<AbstractGameFacade> game);
			std::shared_ptr<AbstractGameFacade> getGameFacade();
	};
}
