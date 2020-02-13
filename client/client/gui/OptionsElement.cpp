#include "OptionsElement.h"
#include "Option.h"
#include "OptionGroup.h"
#include <egui/model/positioning/ValuedPositioningOnScreen.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <cassert>

namespace card {
	OptionsElement::OptionsElement() :
			TitledMenuElement("Tischregeln bearbeiten") {

		chooseColorOnJack_option = std::make_shared<Option>("Farbwahl bei Bube");
		canPutJackOnEveryColor_option = std::make_shared<Option>("Kann Bube auf jede Farbe gelegt werden?");
		addOptionGroup(chooseColorOnJack_option, {canPutJackOnEveryColor_option});

		skipOnEight_option = std::make_shared<Option>("Aussetzen bei 8");
		passSkip_option = std::make_shared<Option>("Aussetzen weitergeben");
		addOptionGroup(skipOnEight_option, {passSkip_option});

		drawTwoOnSeven_option = std::make_shared<Option>("zwei Karten ziehen bei 7");
		passDrawTwo_option = std::make_shared<Option>("+2 weitergeben");
		addOptionGroup(drawTwoOnSeven_option, {passDrawTwo_option});

		directionChangeOnNine_option = std::make_shared<Option>("Richtungswechsel bei 9");
		addStandaloneOption(directionChangeOnNine_option);
		
		contentBox->setSpaceBetweenElements({0.05f, egui::RelativityMode::RELATIVE_IN_PARENT});
		buttonBar = std::make_shared<ContinueBackButtonBar>("Abbrechen", u8"Änderungen übernehmen");
		addChildElement(buttonBar);
		buttonBar->setOwnPositioning(
			std::make_shared<egui::CenterXInParentWrapper>(
				std::make_shared<egui::RelativePositioningOnScreen>(
					0.0f, 0.9f
				)
			)
		);
	}
	void OptionsElement::addStandaloneOption(std::shared_ptr<Option> o) {
		contentBox->addChildElement(o);
	}
	void OptionsElement::addOptionGroup(std::shared_ptr<Option> rootOption, std::initializer_list<std::shared_ptr<Option>> children) {
		auto group = std::make_shared<OptionGroup>(rootOption, OptionGroup::DISABLE_WHEN_PARENT_IS_NOT_TOGGLED);
		for(auto& c : children) {
			group->addChildOption(c);
		}
		contentBox->addChildElement(group);
		group->getAbsYMargin();
	}
	void OptionsElement::addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addBackBtnEventHandler(handler);
	}
	void OptionsElement::addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler) {
		buttonBar->addContinueBtnEventHandler(handler);
	}
	void OptionsElement::loadOptions(RoomOptions& roomOptions) {
		chooseColorOnJack_option->set(roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK));
		canPutJackOnEveryColor_option->set(roomOptions.getOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR));
		skipOnEight_option->set(roomOptions.getOption(Options::SKIP_ON_EIGHT));
		passSkip_option->set(roomOptions.getOption(Options::PASS_SKIP));
		drawTwoOnSeven_option->set(roomOptions.getOption(Options::DRAW_TWO_ON_SEVEN));
		passDrawTwo_option->set(roomOptions.getOption(Options::PASS_DRAW_TWO));
		directionChangeOnNine_option->set(roomOptions.getOption(Options::DIRECTION_CHANGE_ON_NINE));

		assert(roomOptions.getAmountOfOptions() == 7);
	}

	RoomOptions OptionsElement::getOptions() {
		RoomOptions options;
		options.setOption(Options::CHOOSE_COLOR_ON_JACK, chooseColorOnJack_option->isToggled());
		options.setOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR, canPutJackOnEveryColor_option->isToggled());
		options.setOption(Options::SKIP_ON_EIGHT, skipOnEight_option->isToggled());
		options.setOption(Options::PASS_SKIP, passSkip_option->isToggled());
		options.setOption(Options::DRAW_TWO_ON_SEVEN, drawTwoOnSeven_option->isToggled());
		options.setOption(Options::PASS_DRAW_TWO, passDrawTwo_option->isToggled());
		options.setOption(Options::DIRECTION_CHANGE_ON_NINE, directionChangeOnNine_option->isToggled());

		return options;
	}
	
}