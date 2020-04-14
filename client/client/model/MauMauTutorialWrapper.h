#pragma once
#include "MauMauTutorial.h"
#include "ShouldDisplayTutorial.h"

namespace card {
	class MauMauTutorialWrapper {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			MauMauTutorial tutorial;
			ShouldDisplayTutorial shouldDisplayTutorial;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MauMauTutorialWrapper(ProxyMauMauGameData& gameData, MauMauGameAccessorFromClient& accessorFromClient);
	};
}