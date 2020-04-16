#pragma once
#include "ShouldDisplayTutorialInterface.h"
#include "../system/RegistryKey.h"

namespace card {
	class ShouldDisplayTutorial : public ShouldDisplayTutorialInterface {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static std::wstring const SUBKEY;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            RegistryKey registryKey;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ShouldDisplayTutorial();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            bool shouldDisplay(TutorialMessage msg);
            bool shouldDisplayAndClear(TutorialMessage msg);
            void clear(TutorialMessage msg);

        private:
            std::wstring getKeyValue(TutorialMessage msg);

	};
}