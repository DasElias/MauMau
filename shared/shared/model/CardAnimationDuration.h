#pragma once
#include <cstddef>

namespace card {
	static int const INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS = 250;
	static int const INITIAL_DRAW_DURATION_PER_CARD_MS = 1000;
	static int const INITIAL_DRAW_DURATION_PLAYCARDSTACK_MS = 1000;

	int getDelayForPlayCardStack(std::size_t amountOfPlayers, std::size_t amountOfHandCardsPerPlayer);
	int getDurationUntilInitialCardsAreDistributed(std::size_t amountOfPlayers, std::size_t amountOfHandCardsPerPlayer);
}