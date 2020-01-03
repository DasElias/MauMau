#pragma once
#include "IntegerOptions.h"

namespace card {
	// composition
	class RoomOptions : IntegerOptions {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static std::string const CHOOSE_CARD_INDEX_ON_JACK;
            static std::string const SKIP_ON_EIGHT;
            static std::string const DRAW_TWO_CARDS_ON_SEVEN;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            RoomOptions(std::map<std::string, int> integerOptions = {});

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setAllOptions(std::map<std::string, int> integerOptions);
            std::map<std::string, int> getAllOptions() const;
            
            bool chooseCardIndexOnJack() const;
            void setChooseCardIndexOnJack(bool flag);
            bool skipOnEight() const;
            void setSkipOnEight(bool flag);
            bool drawTwoCardsOnSeven() const;
            void setDrawTwoCardsOnSeven(bool flag);

	};
}