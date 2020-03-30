#pragma once
#include <cstddef>

namespace card {
	static int const PLAY_DURATION_MS = 750;
	static int const DRAW_DURATION_MS = 750;
	static int const DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS = 300;
	// card is drawed and played in the same turn
	static int const DELAY_BETWEEN_DRAW_AND_PLAY = 750;

	static int const INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS = 250;
	static int const INITIAL_DRAW_DURATION_PER_CARD_MS = 1000;
	static int const INITIAL_DRAW_DURATION_PLAYCARDSTACK_MS = 1000;

	int getDelayUntilTwoCardsAreDrawed();

	int getDelayUntilPlayStackCanBeInitialized(std::size_t amountOfPlayers, std::size_t amountOfHandCardsPerPlayer);
	int getDurationUntilInitialCardsAreDistributed(std::size_t amountOfPlayers, std::size_t amountOfHandCardsPerPlayer);
}