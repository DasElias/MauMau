#pragma once
#include "ShouldDisplayTutorialInterface.h"
#include "../system/RegistryKey.h"
#include <vector>
#include <set>

namespace card {
	class ShouldDisplayTutorial : public ShouldDisplayTutorialInterface {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static std::wstring const DEFAULT_SUBKEY;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            RegistryKey registryKey;
            std::set<TutorialMessage> shouldntDisplayCache;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ShouldDisplayTutorial(std::wstring subkey = DEFAULT_SUBKEY);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            bool shouldDisplay(TutorialMessage msg);
            bool shouldDisplayAndClear(TutorialMessage msg);
            void clear(TutorialMessage msg);

        private:
            std::wstring getKeyValue(TutorialMessage msg);
            bool isInShouldntDisplayCache(TutorialMessage msg);
             

	};
}