#include "State.h"

#include <shared/utils/TimeUtils.h>
namespace card {

	State::State(StateManager& stateManager) :
			stateManager(stateManager) {
	}

	StateManager& State::getStateMananger() {
		return stateManager;
	}

	void State::resetTimeStateWasEntered() {
		this->timeStateWasEntered = getMilliseconds();
	}

	void State::onStateEnter() {
		this->timeStateWasEntered = getMilliseconds();
	}

	void State::onStateExit() {
		// empty method body
	}

	long long State::getTimeStateWasEntered() const {
		return timeStateWasEntered;
	}

}
