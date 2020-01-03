#include "CardStack.h"
#include <stdexcept>
#include <algorithm>
#include <random>
#include "../utils/MathUtils.h"

namespace card {
	CardStack::CardStack(std::initializer_list<Card> defaultCards) :
			CardCollection(defaultCards) {
	}

	void CardStack::reorder() {	
		// empty method body
	}
	void CardStack::fillWithCardDeckAndShuffle() {
		clear();
		// first card is NULLCARD
		for(int card = 1; card <= Card::MAX_CARDS; card++) {
			addFromPlain({card});
		}

		shuffle();
	}
	void CardStack::shuffle() {
		static auto rng = std::default_random_engine(getRandomDevice()());

		std::shuffle(std::begin(cards), std::end(cards), rng);
	}
}