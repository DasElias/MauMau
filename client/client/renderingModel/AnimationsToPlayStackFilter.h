#pragma once
#include "../model/ProxyMauMauGame.h"

namespace card {
	namespace AnimationsToPlayStackFilterer {
		std::vector<CardAnimation> getAnimationsFromParticularHandCards(ProxyMauMauGame& game, const CardAnimator& handCards);
	}
}