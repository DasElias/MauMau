#pragma once
#include "TitledMenuElement.h"
#include "BoolOptionElement.h"
#include "IntegerOptionElement.h"
#include "ContinueBackButtonBar.h"
#include <shared/model/RoomOptions.h>

namespace card {
	class BasicOptionsElement : public TitledMenuElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:            
            std::shared_ptr<BoolOptionElement> haveToMau_option;
            std::shared_ptr<BoolOptionElement> chooseColorOnJack_option;
            std::shared_ptr<BoolOptionElement> canPutJackOnEveryColor_option;
            std::shared_ptr<BoolOptionElement> canPutJackOnJack_option;
            std::shared_ptr<BoolOptionElement> skipOnEight_option;
            std::shared_ptr<BoolOptionElement> passSkip_option;
            std::shared_ptr<BoolOptionElement> drawTwoOnSeven_option;
            std::shared_ptr<BoolOptionElement> passDrawTwo_option;
            std::shared_ptr<BoolOptionElement> directionChangeOnNine_option;
            std::shared_ptr<IntegerOptionElement> amountOfStartCards_option;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        protected:
            BasicOptionsElement(std::string title);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void loadOptions(RoomOptions& roomOptions);
            RoomOptions getOptions();

        protected:
            void disableIntegerOptions(bool isDisabled);

        private:
            void addStandaloneOption(std::shared_ptr<BoolOptionElement> o);
            void addOptionGroup(std::shared_ptr<BoolOptionElement> rootOption, std::initializer_list<std::shared_ptr<BoolOptionElement>> children);

	};
}