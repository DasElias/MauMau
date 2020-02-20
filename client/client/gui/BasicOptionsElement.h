#pragma once
#include "TitledMenuElement.h"
#include "Option.h"
#include "ContinueBackButtonBar.h"
#include <shared/model/RoomOptions.h>

namespace card {
	class BasicOptionsElement : public TitledMenuElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:            
            std::shared_ptr<Option> chooseColorOnJack_option;
            std::shared_ptr<Option> canPutJackOnEveryColor_option;
            std::shared_ptr<Option> canPutJackOnJack_option;
            std::shared_ptr<Option> skipOnEight_option;
            std::shared_ptr<Option> passSkip_option;
            std::shared_ptr<Option> drawTwoOnSeven_option;
            std::shared_ptr<Option> passDrawTwo_option;
            std::shared_ptr<Option> directionChangeOnNine_option;

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

        private:
            void addStandaloneOption(std::shared_ptr<Option> o);
            void addOptionGroup(std::shared_ptr<Option> rootOption, std::initializer_list<std::shared_ptr<Option>> children);

	};
}