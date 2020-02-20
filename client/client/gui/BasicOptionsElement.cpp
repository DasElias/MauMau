#include "BasicOptionsElement.h"
#include "Option.h"
#include "OptionGroup.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <cassert>

namespace card {
	BasicOptionsElement::BasicOptionsElement(std::string title) :
			TitledMenuElement(title) {

		chooseColorOnJack_option = std::make_shared<Option>("Farbwahl bei Bube");
		canPutJackOnEveryColor_option = std::make_shared<Option>("Kann Bube auf jede Farbe gelegt werden?");
		canPutJackOnJack_option = std::make_shared<Option>("Kann Bube auf Bube gelegt werden?");
		addOptionGroup(chooseColorOnJack_option, {canPutJackOnEveryColor_option, canPutJackOnJack_option});

		skipOnEight_option = std::make_shared<Option>("Aussetzen bei 8");
		passSkip_option = std::make_shared<Option>("Aussetzen weitergeben");
		addOptionGroup(skipOnEight_option, {passSkip_option});

		drawTwoOnSeven_option = std::make_shared<Option>("zwei Karten ziehen bei 7");
		passDrawTwo_option = std::make_shared<Option>("+2 weitergeben");
		addOptionGroup(drawTwoOnSeven_option, {passDrawTwo_option});

		directionChangeOnNine_option = std::make_shared<Option>("Richtungswechsel bei 9");
		addStandaloneOption(directionChangeOnNine_option);
		
		contentBox->setSpaceBetweenElements({0.05f, egui::RelativityMode::RELATIVE_IN_PARENT});
	}
	void BasicOptionsElement::addStandaloneOption(std::shared_ptr<Option> o) {
		contentBox->addChildElement(o);
	}
	void BasicOptionsElement::addOptionGroup(std::shared_ptr<Option> rootOption, std::initializer_list<std::shared_ptr<Option>> children) {
		auto group = std::make_shared<OptionGroup>(rootOption, OptionGroup::DISABLE_WHEN_PARENT_IS_NOT_TOGGLED);
		for(auto& c : children) {
			group->addChildOption(c);
		}
		contentBox->addChildElement(group);
		group->getAbsYMargin();
	}
	void BasicOptionsElement::loadOptions(RoomOptions& roomOptions) {
		chooseColorOnJack_option->set(roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK));
		canPutJackOnEveryColor_option->set(roomOptions.getOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR));
		canPutJackOnJack_option->set(roomOptions.getOption(Options::CAN_PUT_JACK_ON_JACK));
		skipOnEight_option->set(roomOptions.getOption(Options::SKIP_ON_EIGHT));
		passSkip_option->set(roomOptions.getOption(Options::PASS_SKIP));
		drawTwoOnSeven_option->set(roomOptions.getOption(Options::DRAW_TWO_ON_SEVEN));
		passDrawTwo_option->set(roomOptions.getOption(Options::PASS_DRAW_TWO));
		directionChangeOnNine_option->set(roomOptions.getOption(Options::DIRECTION_CHANGE_ON_NINE));

		assert(roomOptions.getAmountOfOptions() == 8);
	}

	RoomOptions BasicOptionsElement::getOptions() {
		RoomOptions options;
		options.setOption(Options::CHOOSE_COLOR_ON_JACK, chooseColorOnJack_option->isToggled());
		options.setOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR, canPutJackOnEveryColor_option->isToggled());
		options.setOption(Options::CAN_PUT_JACK_ON_JACK, canPutJackOnJack_option->isToggled());
		options.setOption(Options::SKIP_ON_EIGHT, skipOnEight_option->isToggled());
		options.setOption(Options::PASS_SKIP, passSkip_option->isToggled());
		options.setOption(Options::DRAW_TWO_ON_SEVEN, drawTwoOnSeven_option->isToggled());
		options.setOption(Options::PASS_DRAW_TWO, passDrawTwo_option->isToggled());
		options.setOption(Options::DIRECTION_CHANGE_ON_NINE, directionChangeOnNine_option->isToggled());

		assert(options.getAmountOfOptions() == 8);
		return options;
	}
	
}