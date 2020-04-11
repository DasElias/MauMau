#include "HandCardStack.h"
#include <algorithm>

namespace card {
	HandCardStack::HandCardStack(std::vector<Card> cards) :
			CardCollection(cards) {

		reorder();
	}
	void HandCardStack::reorder() {
		std::sort(cards.begin(), cards.end());
	}
}
