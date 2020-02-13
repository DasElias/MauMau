#pragma once
#include "IntegerOptions.h"

namespace card {
    enum class Options : unsigned int {
        CHOOSE_COLOR_ON_JACK,
        CAN_PUT_JACK_ON_EVERY_COLOR,
        SKIP_ON_EIGHT,
        PASS_SKIP,
        DRAW_TWO_ON_SEVEN,
        PASS_DRAW_TWO,
        DIRECTION_CHANGE_ON_NINE
    };
    
	// composition
	class RoomOptions : IntegerOptions {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const OPTIONS_SIZE = 7;

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

            bool getOption(Options key) const;
            void setOption(Options key, bool newValue);
            std::size_t getAmountOfOptions() const;

        private:
            std::string keyToString(Options key) const;

	};
}