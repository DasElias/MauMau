#include "CardAnimationSet.h"
#include "CardAnimationSet.h"
#include <stdexcept>

namespace card {
	CardAnimation CardAnimationSet::getLast() const {
		if(empty()) throw std::runtime_error("Can't get card of an empty card animation set");
		return *begin();
	}
	CardAnimation CardAnimationSet::getFirst() const {
		if(empty()) throw std::runtime_error("Can't get card of an empty card animation set");
		return *rbegin();
	}
}