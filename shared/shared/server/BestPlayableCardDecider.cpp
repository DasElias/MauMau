#include "BestPlayableCardDecider.h"
#include <boost/container/small_vector.hpp>
#include "../model/MauMauCardValueMeanings.h"
#include <algorithm>
#include "../utils/MathUtils.h"

namespace bc = boost::container;
typedef bc::small_vector<card::Card, 10> TemporaryCardVector;

namespace card {
	static void insertInTempStack(Card c, TemporaryCardVector& tempStack) {
		if(c.getValue() == CHANGE_COLOR_VALUE) {
			tempStack.push_back(c);
		} else {
			tempStack.insert(tempStack.begin(), c);
		}
	}

	static Card getBestCardToPlay(std::vector<Card>& playableCards, CardIndex activeCardIndex) {
		TemporaryCardVector cardsOfCurrentIndex;
		TemporaryCardVector cardsOfOtherIndex;

		// shuffle playable cards
		std::shuffle(std::begin(playableCards), std::end(playableCards), getEngine());

		for(auto& c : playableCards) {
			if(c.getCardIndex() == activeCardIndex) insertInTempStack(c, cardsOfCurrentIndex);
			else insertInTempStack(c, cardsOfOtherIndex);
		}

		if(cardsOfCurrentIndex.size() > 0) {
			return cardsOfCurrentIndex[0];
		} else if(cardsOfOtherIndex.size() > 0) {
			return cardsOfOtherIndex[0];
		} else {
			throw std::runtime_error("playableCards are empty");
		}
	}

	static Card getRandomCardToPlay(std::vector<Card>& playableCards) {
		std::size_t randomIndex = randomInRange<std::size_t>(0, playableCards.size() - 1);
		return playableCards[randomIndex];
	}

	Card BestPlayableCardDecider::getCardToPlay(std::vector<Card>& playableCards, CardIndex activeCardIndex) {
		static float const PROBABILITY_FOR_NOT_PLAYING_BEST_OPTION = 0.05f;

		if(randomReal<float>() < PROBABILITY_FOR_NOT_PLAYING_BEST_OPTION) return getRandomCardToPlay(playableCards);
		else return getBestCardToPlay(playableCards, activeCardIndex);
	}
}

