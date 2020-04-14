#pragma once
#include <string>
#include <memory>

namespace card {
    struct RegistryHandleWrapper;

	class RegistryKey {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        protected:
            std::unique_ptr<RegistryHandleWrapper> registryHandleWrapper;

        private:
            bool wasCreatedNewKey_field;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            RegistryKey(std::wstring subKey);
            RegistryKey(const RegistryKey&) = delete;
            ~RegistryKey();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            bool wasCreatedNewKey() const;



	};
}