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
            
            void setDword(std::wstring valueName, uint32_t value);
            uint32_t getDword(std::wstring valueName);
            uint32_t getDwordOrDefault(std::wstring valueName, uint32_t defaultValue);

            void setString(std::wstring valueName, std::string value);
            std::string getString(std::wstring valueName);
            std::string getStringOrDefault(std::wstring valueName, std::string defaultValue);
	};
}