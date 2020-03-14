#include "RoomOptions.h"
#include <stdexcept>
#include <assert.h>

namespace card {
	RoomOptions::RoomOptions() {
		setOption(Options::CHOOSE_COLOR_ON_JACK, true);
		setOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR, false);
		setOption(Options::CAN_PUT_JACK_ON_JACK, false);
		setOption(Options::SKIP_ON_EIGHT, true);
		setOption(Options::PASS_SKIP, false);
		setOption(Options::DRAW_TWO_ON_SEVEN, true);
		setOption(Options::DIRECTION_CHANGE_ON_NINE, false);
		setOption(Options::AMOUNT_OF_START_CARDS, 6);

		assert(getSize() == OPTIONS_SIZE);
	}
	RoomOptions::RoomOptions(std::map<std::string, int> integerOptions) :
			IntegerOptions(integerOptions) {

		if(integerOptions.size() != OPTIONS_SIZE) {
			throw std::runtime_error("Tried to initialize RoomOptions with an incorrect number of option values!");
		}
	}
	void RoomOptions::setAllOptions(std::map<std::string, int> integerOptions) {
		IntegerOptions::setAllOptions(integerOptions);
	}
	std::map<std::string, int> RoomOptions::getAllOptions() const {
		return getData();
	}
	int RoomOptions::getOption(Options key) const {
		return queryOption(keyToString(key));
	}
	void RoomOptions::setOption(Options key, int newValue) {
		IntegerOptions::setOption(keyToString(key), newValue);
	}
	std::size_t RoomOptions::getAmountOfOptions() const {
		return getSize();
	}
	std::string RoomOptions::keyToString(Options key) const {
		return std::to_string(static_cast<unsigned int>(key));
	}
}