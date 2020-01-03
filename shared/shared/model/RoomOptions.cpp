#include "RoomOptions.h"

namespace card {
	std::string const RoomOptions::CHOOSE_CARD_INDEX_ON_JACK = "chooseCardIndexOnJack";
	std::string const RoomOptions::SKIP_ON_EIGHT = "skipOnEight";
	std::string const RoomOptions::DRAW_TWO_CARDS_ON_SEVEN = "drawTwoCardsOnSeven";

	RoomOptions::RoomOptions(std::map<std::string, int> integerOptions) :
			IntegerOptions(integerOptions) {
	}
	void RoomOptions::setAllOptions(std::map<std::string, int> integerOptions) {
		IntegerOptions::setAllOptions(integerOptions);
	}
	std::map<std::string, int> RoomOptions::getAllOptions() const {
		return getData();
	}
	bool RoomOptions::chooseCardIndexOnJack() const {
		return queryOption(CHOOSE_CARD_INDEX_ON_JACK, true);
	}
	void RoomOptions::setChooseCardIndexOnJack(bool flag) {
		setOption(CHOOSE_CARD_INDEX_ON_JACK, flag);
	}
	bool RoomOptions::skipOnEight() const {
		return queryOption(SKIP_ON_EIGHT, true);
	}
	void RoomOptions::setSkipOnEight(bool flag) {
		setOption(SKIP_ON_EIGHT, flag);
	}
	bool RoomOptions::drawTwoCardsOnSeven() const {
		return queryOption(DRAW_TWO_CARDS_ON_SEVEN, true);
	}
	void RoomOptions::setDrawTwoCardsOnSeven(bool flag) {
		setOption(DRAW_TWO_CARDS_ON_SEVEN, flag);
	}
}