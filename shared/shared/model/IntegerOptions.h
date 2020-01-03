#pragma once

#include <map>
#include <string>

namespace card {
	class IntegerOptions {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::map<std::string, int> integerOptions; 

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IntegerOptions(std::map<std::string, int> integerOptions = {});
			IntegerOptions(const IntegerOptions&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setAllOptions(std::map<std::string, int> integerOptions);
			void setOption(std::string key, int value);
			void setOptionIfNotExist(std::string key, int value);
			bool doesOptionExist(std::string key) const;
			int queryOption(std::string key) const;
			int queryOption(std::string key, int returnIfNotExistant) const;
			std::map<std::string, int> getData() const;

	};
}