#include "AnimationsToPlayStackFilter.h"

namespace card {
	std::vector<CardAnimation> AnimationsToPlayStackFilterer::getAnimationsFromParticularHandCards(ProxyMauMauGame& game, const CardAnimator& handCards) {
		std::vector<CardAnimation> relevantAnimations;

		auto& playStack = game.getPlayStack();
		for(auto& animation : playStack.getCardAnimations()) {
			auto& sourceStack = animation.source.get();
			if(sourceStack.equalsId(handCards)) relevantAnimations.push_back(animation);
		}

		return relevantAnimations;
	}
}