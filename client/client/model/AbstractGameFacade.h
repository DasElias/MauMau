#pragma once
#include "ProxyMauMauGame.h"
#include "ProxyRoom.h"

namespace card {
	class AbstractGameFacade {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			AbstractGameFacade() = default;
			AbstractGameFacade(const AbstractGameFacade&) = delete;
			virtual ~AbstractGameFacade() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual bool isGameRunning() const =0;
			virtual ProxyMauMauGame& getGame() =0;
			virtual ProxyRoom& getRoom() =0;
	
	};
}