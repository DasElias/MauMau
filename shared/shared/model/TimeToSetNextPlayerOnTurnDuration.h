#pragma once
#include "CardStack.h"
#include "RoomOptions.h"

namespace card {
	int getTimeToSetNextPlayerOnTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn, int cardsToDrawDueToPlusTwo, const RoomOptions& options);
	int getTimeToEndCurrentTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn);
}