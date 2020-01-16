#include "CardAnimationDuration.h"

namespace card {
	int getDelayUntilTwoCardsAreDrawed() {
		return PLAY_DURATION_MS + DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS + DRAW_DURATION_MS;
	}
	int getDelayUntilPlayStackCanBeInitialized(std::size_t amountOfPlayers, std::size_t amountOfHandCardsPerPlayer) {
		auto result = amountOfPlayers * amountOfHandCardsPerPlayer * INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS + INITIAL_DRAW_DURATION_PER_CARD_MS;
		return static_cast<int>(result);
	}
	int getDurationUntilInitialCardsAreDistributed(std::size_t amountOfPlayers, std::size_t amountOfHandCardsPerPlayer) {
		return getDelayUntilPlayStackCanBeInitialized(amountOfPlayers, amountOfHandCardsPerPlayer) + INITIAL_DRAW_DURATION_PLAYCARDSTACK_MS;
	}
}