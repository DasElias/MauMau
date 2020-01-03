#pragma once
#include "CardCollection.h"

namespace card {
	class CardStack : public CardCollection {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardStack(std::initializer_list<Card> = {});

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			void reorder() override;

		public:
			void fillWithCardDeckAndShuffle();
			void shuffle();
			
	};
}