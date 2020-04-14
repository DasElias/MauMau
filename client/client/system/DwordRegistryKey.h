#pragma once
#include "RegistryKey.h"

namespace card {
    class DwordRegistryKey : public RegistryKey {
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            DwordRegistryKey(std::wstring subKey);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setValue(std::wstring valueName, uint32_t value);
            uint32_t getValue(std::wstring valueName);
            uint32_t getValueOrDefault(std::wstring valueName, uint32_t defaultValue);

    };
}