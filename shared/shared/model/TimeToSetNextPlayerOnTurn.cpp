#include "TimeToSetNextPlayerOnTurnDuration.h"
#include "CardAnimationDuration.h"
#include "MauMauCardValueMeanings.h"
#include "../utils/Logger.h"

namespace card {
	int getTimeToSetNextPlayerOnTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn) {
		int delay = getTimeToEndCurrentTurn(playCardStackSize, lastCardOnPlayCardStack, wasCardPlayedThisTurn, wasCardDrawedThisTurn);

		if(wasCardPlayedThisTurn && playCardStackSize > 1 && lastCardOnPlayCardStack.getValue() == DRAW_2_VALUE) {
			delay += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS + DRAW_DURATION_MS;
		}
		if(wasCardDrawedThisTurn && !wasCardPlayedThisTurn) {
			delay += DRAW_DURATION_MS;
		}
		if(wasCardPlayedThisTurn) {
			delay += PLAY_DURATION_MS;
		}
		return delay;
	}
	int getTimeToEndCurrentTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn) {
		int delay = 0;

		if(wasCardPlayedThisTurn && wasCardDrawedThisTurn) {
			delay += DRAW_DURATION_MS;
			delay += DELAY_BETWEEN_DRAW_AND_PLAY;
		}

		return delay;
	}
}