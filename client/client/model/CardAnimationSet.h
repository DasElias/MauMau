#pragma once
#include "CardAnimation.h"
#include <set>

namespace card {
	class CardAnimationSet : public std::set<CardAnimation> {
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			CardAnimation getLast() const;
			CardAnimation getFirst() const;
	};
}