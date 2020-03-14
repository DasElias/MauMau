#include "UsernameInputFieldCharacterFilter.h"

namespace card {
	UsernameInputFieldCharacterFilter::UsernameInputFieldCharacterFilter(std::shared_ptr<egui::InputField> inputField) :
			inputField(inputField) {
	}
	bool UsernameInputFieldCharacterFilter::operator()(unsigned int enteredChar) const {
		return inputField->getText().size() >= USERNAME_MAX_LENGTH || !isCharacterValid(enteredChar);
	}
	bool UsernameInputFieldCharacterFilter::isCharacterValid(unsigned int c) const {
		return isalnum(c) || c == '.' || c == '_' || c == '-';
	}
}