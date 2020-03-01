#include "CardAnimation.h"
#include <shared/model/CardCollection.h>

namespace card {
	CardAnimation::CardAnimation(long long animationStartTime, int duration, CardAnimator& source, Card mutatesTo, int indexInSourceStack) :
			animationStartTime(animationStartTime),
			duration(duration),
			source(source),
			mutatesTo(mutatesTo),
			indexInSourceStack(indexInSourceStack) {
	}
	bool CardAnimation::operator<(const CardAnimation& other) const {
		return this->animationStartTime + this->duration < other.animationStartTime + other.duration;
	}
	bool CardAnimation::operator==(const CardAnimation& other) const {
		return animationStartTime == other.animationStartTime && duration == other.duration && mutatesTo == other.mutatesTo;
	}
}