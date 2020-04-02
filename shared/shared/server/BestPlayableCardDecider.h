#pragma once
#include "../model/Card.h"
#include <vector>


namespace card {
	namespace BestPlayableCardDecider {
		// the playableCards vector may be shuffled afterwise
		Card getCardToPlay(std::vector<Card>& playableCards, CardIndex activeCardIndex);
	}
}