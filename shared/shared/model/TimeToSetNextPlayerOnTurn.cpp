#include "TimeToSetNextPlayerOnTurnDuration.h"
#include "CardAnimationDuration.h"
#include "MauMauCardValueMeanings.h"
#include "../utils/Logger.h"

namespace card {
	int getTimeToSetNextPlayerOnTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn) {
		int delay = 0;
		if(playCardStackSize > 1 && lastCardOnPlayCardStack.getValue() == DRAW_2_VALUE) {
			delay += getDelayUntilTwoCardsAreDrawed();
		}
		if(playCardStackSize > 1 && lastCardOnPlayCardStack.getValue() == SKIP_VALUE) {
			delay += SKIP_ANIMATION_DURATION_MS;
		}
		if(wasCardDrawedThisTurn && wasCardPlayedThisTurn) {
			delay += DELAY_BETWEEN_DRAW_AND_PLAY;
		}
		if(wasCardDrawedThisTurn) {
			delay += DRAW_DURATION_MS;
		}
		if(wasCardPlayedThisTurn) {
			delay += PLAY_DURATION_MS;
		}
		return delay;
	}
}