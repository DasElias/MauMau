#include "BasicOptionsElement.h"
#include "BoolOptionElement.h"
#include "OptionGroup.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <cassert>
#include <shared/model/OptionLimitValues.h>

namespace card {
	BasicOptionsElement::BasicOptionsElement(std::string title) :
			TitledMenuElement(title) {

		haveToMau_option = std::make_shared<BoolOptionElement>(u8"Mau-Knopf muss vor dem Spielen der vorletzten Karte betätigt werden");
		addStandaloneOption(haveToMau_option);

		chooseColorOnJack_option = std::make_shared<BoolOptionElement>("Farbwahl bei Bube");
		canPutJackOnEveryColor_option = std::make_shared<BoolOptionElement>("Kann Bube auf jede Farbe gelegt werden?");
		canPutJackOnJack_option = std::make_shared<BoolOptionElement>("Kann Bube auf Bube gelegt werden?");
		addOptionGroup(chooseColorOnJack_option, {canPutJackOnEveryColor_option, canPutJackOnJack_option});

		skipOnEight_option = std::make_shared<BoolOptionElement>("Aussetzen bei 8");
		passSkip_option = std::make_shared<BoolOptionElement>("Aussetzen weiterreichen");
		addOptionGroup(skipOnEight_option, {passSkip_option});

		drawTwoOnSeven_option = std::make_shared<BoolOptionElement>("zwei Karten ziehen bei 7");
		passDrawTwo_option = std::make_shared<BoolOptionElement>("zwei Karten ziehen weiterreichen");
		addOptionGroup(drawTwoOnSeven_option, {passDrawTwo_option});

		directionChangeOnNine_option = std::make_shared<BoolOptionElement>("Richtungswechsel bei 9");
		addStandaloneOption(directionChangeOnNine_option);

		amountOfStartCards_option = std::make_shared<IntegerOptionElement>("Anzahl Startkarten", 6, MIN_AMOUNT_OF_START_CARDS, MAX_AMOUNT_OF_START_CARDS);
		contentBox->addChildElement(amountOfStartCards_option);

		amountOfStartCardDecks_option = std::make_shared<IntegerOptionElement>("Anzahl Kartendecks auf Ziehstapel", 1, MIN_AMOUNT_OF_CARD_DECKS, MAX_AMOUNT_OF_CARD_DECKS);
		contentBox->addChildElement(amountOfStartCardDecks_option);
		
		contentBox->setSpaceBetweenElements({0.05f, egui::RelativityMode::RELATIVE_IN_PARENT});
	}
	void BasicOptionsElement::addStandaloneOption(std::shared_ptr<BoolOptionElement> o) {
		contentBox->addChildElement(o);
	}
	void BasicOptionsElement::addOptionGroup(std::shared_ptr<BoolOptionElement> rootOption, std::initializer_list<std::shared_ptr<BoolOptionElement>> children) {
		auto group = std::make_shared<OptionGroup>(rootOption, OptionGroup::DISABLE_WHEN_PARENT_IS_NOT_TOGGLED);
		for(auto& c : children) {
			group->addChildOption(c);
		}
		contentBox->addChildElement(group);
		group->getAbsYMargin();
	}
	void BasicOptionsElement::loadOptions(RoomOptions& roomOptions) {
		haveToMau_option->set(roomOptions.getOption(Options::HAVE_TO_MAU));
		chooseColorOnJack_option->set(roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK));
		canPutJackOnEveryColor_option->set(roomOptions.getOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR));
		canPutJackOnJack_option->set(roomOptions.getOption(Options::CAN_PUT_JACK_ON_JACK));
		skipOnEight_option->set(roomOptions.getOption(Options::SKIP_ON_EIGHT));
		passSkip_option->set(roomOptions.getOption(Options::PASS_SKIP));
		drawTwoOnSeven_option->set(roomOptions.getOption(Options::DRAW_TWO_ON_SEVEN));
		passDrawTwo_option->set(roomOptions.getOption(Options::PASS_DRAW_TWO));
		directionChangeOnNine_option->set(roomOptions.getOption(Options::DIRECTION_CHANGE_ON_NINE));
		amountOfStartCards_option->setValue(roomOptions.getOption(Options::AMOUNT_OF_START_CARDS));
		amountOfStartCardDecks_option->setValue(roomOptions.getOption(Options::AMOUNT_OF_START_CARD_DECKS));

		assert(roomOptions.getAmountOfOptions() == 11);
	}

	RoomOptions BasicOptionsElement::getOptions() {
		RoomOptions options;
		options.setOption(Options::HAVE_TO_MAU, haveToMau_option->isToggled());
		options.setOption(Options::CHOOSE_COLOR_ON_JACK, chooseColorOnJack_option->isToggled());
		options.setOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR, canPutJackOnEveryColor_option->isToggled());
		options.setOption(Options::CAN_PUT_JACK_ON_JACK, canPutJackOnJack_option->isToggled());
		options.setOption(Options::SKIP_ON_EIGHT, skipOnEight_option->isToggled());
		options.setOption(Options::PASS_SKIP, passSkip_option->isToggled());
		options.setOption(Options::DRAW_TWO_ON_SEVEN, drawTwoOnSeven_option->isToggled());
		options.setOption(Options::PASS_DRAW_TWO, passDrawTwo_option->isToggled());
		options.setOption(Options::DIRECTION_CHANGE_ON_NINE, directionChangeOnNine_option->isToggled());
		options.setOption(Options::AMOUNT_OF_START_CARDS, amountOfStartCards_option->getValue());
		options.setOption(Options::AMOUNT_OF_START_CARD_DECKS, amountOfStartCardDecks_option->getValue());

		assert(options.getAmountOfOptions() == 11);
		return options;
	}

	void BasicOptionsElement::disableIntegerOptions(bool disabledState) {
		amountOfStartCards_option->setDisabled(disabledState);
	}
	
}