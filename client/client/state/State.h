#pragma once
#include "StateManager.h"

namespace card {
	class State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			long long timeStateWasEntered = 0;

		protected:
			StateManager& stateManager;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			State(StateManager& stateManager);
			State(const State&) = delete;
			virtual ~State() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			StateManager& getStateMananger();
			void resetTimeStateWasEntered();

		public:
			virtual void updateAndRender(float delta) =0;
			virtual void onStateEnter();
			virtual void onStateExit();

			long long getTimeStateWasEntered() const;

	};
}
