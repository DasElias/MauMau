#include "CardAnimationCollection.h"
#include <stdexcept>
#include "../utils/InsertionSort.h"

namespace card {
	void CardAnimationCollection::insertAnimation(CardAnimation animationToInsert) {
		insertionSortAscending(*this, animationToInsert);
	}
	void CardAnimationCollection::removeAnimation(CardAnimation animationToRemove) {
		erase(std::find(begin(), end(), animationToRemove));
	}
	CardAnimation CardAnimationCollection::getLast() const {
		if(empty()) throw std::runtime_error("Can't get card of an empty CardAnimationCollection");
		return *begin();
	}
	CardAnimation CardAnimationCollection::getFirst() const {
		if(empty()) throw std::runtime_error("Can't get card of an empty CardAnimationCollection");
		return *rbegin();
	}
}