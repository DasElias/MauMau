#include "RoomOptions.h"
#include <stdexcept>
#include <assert.h>
#include "OptionLimitValues.h"

namespace card {
	RoomOptions::RoomOptions() {
		setOption(Options::HAVE_TO_MAU, true);
		setOption(Options::CHOOSE_COLOR_ON_JACK, true);
		setOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR, false);
		setOption(Options::CAN_PUT_JACK_ON_JACK, false);
		setOption(Options::SKIP_ON_EIGHT, true);
		setOption(Options::PASS_SKIP, true);
		setOption(Options::DRAW_TWO_ON_SEVEN, true);
		setOption(Options::PASS_DRAW_TWO, true);
		setOption(Options::DIRECTION_CHANGE_ON_NINE, false);
		setOption(Options::AMOUNT_OF_START_CARDS, 6);
		setOption(Options::AMOUNT_OF_START_CARD_DECKS, 1);

		assert(getSize() == OPTIONS_SIZE);
	}
	RoomOptions::RoomOptions(std::map<std::string, int> integerOptions) :
			IntegerOptions(integerOptions) {

		if(integerOptions.size() != OPTIONS_SIZE) {
			throw std::runtime_error("Tried to initialize RoomOptions with an incorrect number of option values!");
		}

		validate();
	}
	void RoomOptions::setAllOptions(std::map<std::string, int> integerOptions) {
		IntegerOptions::setAllOptions(integerOptions);
		validate();
	}
	std::map<std::string, int> RoomOptions::getAllOptions() const {
		return getData();
	}
	int RoomOptions::getOption(Options key) const {
		return queryOption(keyToString(key));
	}
	void RoomOptions::setOption(Options key, int newValue) {
		IntegerOptions::setOption(keyToString(key), newValue);
		validate();
	}
	std::size_t RoomOptions::getAmountOfOptions() const {
		return getSize();
	}
	std::string RoomOptions::keyToString(Options key) const {
		return std::to_string(static_cast<unsigned int>(key));
	}
	void RoomOptions::validate() const {
		// validate AMOUNT_OF_START_CARDS
		int amountOfStartCards = queryOption(keyToString(Options::AMOUNT_OF_START_CARDS), MIN_AMOUNT_OF_START_CARDS);
		if(amountOfStartCards < MIN_AMOUNT_OF_START_CARDS || amountOfStartCards > MAX_AMOUNT_OF_START_CARDS) throw std::runtime_error("invalid value of AMOUNT_OF_START_CARDS");

		int amountOfStartCardDecks = queryOption(keyToString(Options::AMOUNT_OF_START_CARD_DECKS), MIN_AMOUNT_OF_CARD_DECKS);
		if(amountOfStartCardDecks < MIN_AMOUNT_OF_CARD_DECKS || amountOfStartCardDecks > MAX_AMOUNT_OF_CARD_DECKS) throw std::runtime_error("invalid value of AMOUNT_OF_START_CARD_DECKS");

	}
}