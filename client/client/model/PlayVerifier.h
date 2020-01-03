#pragma once
#include <shared/model/Card.h>

namespace card {
	class PlayVerifier {
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual bool canChangeColor(Card playedCard) const =0;
	};
}