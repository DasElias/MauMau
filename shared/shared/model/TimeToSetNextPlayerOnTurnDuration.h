#pragma once
#include "CardStack.h"
#include "RoomOptions.h"

namespace card {
	int getTimeToSetNextPlayerOnTurn(bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn, int cardsToDrawDueToPlusTwo);
	int getTimeToEndCurrentTurn(bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn);
}