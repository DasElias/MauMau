#include "CardAnimation.h"
#include <shared/model/CardCollection.h>

namespace card {
	CardAnimation::CardAnimation(long long animationStartTime, int duration, CardAnimator& source, Card mutatesTo) :
			animationStartTime(animationStartTime),
			duration(duration),
			source(source),
			mutatesTo(mutatesTo) {
	}
	bool CardAnimation::operator<(const CardAnimation& other) const {
		return this->animationStartTime < other.animationStartTime;
	}
}