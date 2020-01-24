#pragma once
#include "CardStack.h"

namespace card {
	int getTimeToSetNextPlayerOnTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn);
	int getTimeToEndCurrentTurn(std::size_t playCardStackSize, Card lastCardOnPlayCardStack, bool wasCardPlayedThisTurn, bool wasCardDrawedThisTurn);
}