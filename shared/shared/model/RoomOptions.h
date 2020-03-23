#pragma once
#include "IntegerOptions.h"

namespace card {
    enum class Options : unsigned int {
        HAVE_TO_MAU,
        CHOOSE_COLOR_ON_JACK,
        CAN_PUT_JACK_ON_EVERY_COLOR,
        CAN_PUT_JACK_ON_JACK,
        SKIP_ON_EIGHT,
        PASS_SKIP,
        DRAW_TWO_ON_SEVEN,
        PASS_DRAW_TWO,
        DIRECTION_CHANGE_ON_NINE,
        AMOUNT_OF_START_CARDS
    };
    
	// composition
	class RoomOptions : IntegerOptions {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static int const OPTIONS_SIZE = 10;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            RoomOptions();
            RoomOptions(std::map<std::string, int> integerOptions);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setAllOptions(std::map<std::string, int> integerOptions);
            std::map<std::string, int> getAllOptions() const;

            int getOption(Options key) const;
            void setOption(Options key, int newValue);
            std::size_t getAmountOfOptions() const;

        private:
            std::string keyToString(Options key) const;

	};
}