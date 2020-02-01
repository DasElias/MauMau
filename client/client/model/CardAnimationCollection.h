#pragma once
#include "CardAnimation.h"
#include <set>

namespace card {
	class CardAnimationCollection : public std::vector<CardAnimation> {
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void insertAnimation(CardAnimation animationToInsert);
			void removeAnimation(CardAnimation animationToRemove);

			// would be the last card added to the destination
			CardAnimation getLast() const;
			// would be the first card added to the source
			CardAnimation getFirst() const;
	};
}