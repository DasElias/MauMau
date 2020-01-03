#include "IntegerOptions.h"
#include <stdexcept>
#include <vector>

namespace card {
	IntegerOptions::IntegerOptions(std::map<std::string, int> integerOptions) :
			integerOptions(integerOptions) {
	}
	
	void IntegerOptions::setAllOptions(std::map<std::string, int> options) {
		this->integerOptions = options;
	}
	void IntegerOptions::setOption(std::string key, int value) {
		integerOptions[key] = value;
	}
	void IntegerOptions::setOptionIfNotExist(std::string key, int value) {
		if(! doesOptionExist(key)) setOption(key, value);
	}
	bool IntegerOptions::doesOptionExist(std::string key) const {
		return integerOptions.find(key) != integerOptions.end();
	}
	int IntegerOptions::queryOption(std::string key) const {
		if(doesOptionExist(key)) return integerOptions.at(key);
		throw std::runtime_error("Option does not exist!");
	}
	int IntegerOptions::queryOption(std::string key, int returnIfNotExistant) const {
		return (doesOptionExist(key)) ? queryOption(key) : returnIfNotExistant;
	}
	std::map<std::string, int> IntegerOptions::getData() const {
		return integerOptions;
	}
}