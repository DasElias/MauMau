#pragma once
#include <shared/model/Card.h>

namespace card {
	class PlayVerifier {
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual bool canPlay(Card card) const =0;
	};
}