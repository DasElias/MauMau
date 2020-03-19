#include "TimeToSetNextPlayerOnTurnDuration.h"
#include "CardAnimationDuration.h"
#include "MauMauCardValueMeanings.h"
#include "../utils/Logger.h"

namespace card {
	int getTimeToSetNextPlayerOnTurn(bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn, int cardsToDrawDueToPlusTwo) {
		int delay = getTimeToEndCurrentTurn(wasCardPlayedThisTurn, wasCardDrawedThisTurn);

		if(cardsToDrawDueToPlusTwo > 0) {
			delay += DRAW_DURATION_MS + (cardsToDrawDueToPlusTwo - 1) * DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
		if(wasCardDrawedThisTurn && !wasCardPlayedThisTurn) {
			delay += DRAW_DURATION_MS;
		}
		if(wasCardPlayedThisTurn) {
			delay += PLAY_DURATION_MS;
		}
		return delay;
	}
	int getTimeToEndCurrentTurn(bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn) {
		int delay = 0;

		if(wasCardPlayedThisTurn && wasCardDrawedThisTurn) {
			delay += DRAW_DURATION_MS;
			delay += DELAY_BETWEEN_DRAW_AND_PLAY;
		}

		return delay;
	}
}