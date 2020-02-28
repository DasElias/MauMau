#include "OptionGroup.h"

namespace card {
	OptionGroup::ShouldDisableChildFunction const OptionGroup::DISABLE_NEVER = [](bool isRootToggled) {
		return false;
	};
	OptionGroup::ShouldDisableChildFunction const OptionGroup::DISABLE_WHEN_PARENT_IS_TOGGLED = [](bool isRootToggled) {
		return isRootToggled;
	};
	OptionGroup::ShouldDisableChildFunction const OptionGroup::DISABLE_WHEN_PARENT_IS_NOT_TOGGLED = [](bool isRootToggled) {
		return !isRootToggled;
	};

	int const OptionGroup::SPACE_BETWEEN_ELEMS_PX = 5;


	OptionGroup::OptionGroup(std::shared_ptr<BoolOptionElement> rootOption, ShouldDisableChildFunction shouldDisableChildFunc) :
			VBox(),
			rootOption(rootOption),
			shouldDisableChildFunc(shouldDisableChildFunc) {

		setSpaceBetweenElements({SPACE_BETWEEN_ELEMS_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		this->addChildElement(rootOption);
		rootOption->getToggleEventManager().addEventHandler({[this](egui::ToggleEvent& e) {
			for(auto& childOption : children) {
				updateDisabledStateOfChild(childOption, e.isActiveNow());
			}
			updatePreferredHeight();
		}});
		updatePreferredHeight();
	}
	void OptionGroup::addChildOption(std::shared_ptr<BoolOptionElement> childOption) {
		children.push_back(childOption);
		this->addChildElement(childOption);

		int const CHILD_INTENDATION_PX = 25;
		childOption->setXTranslation(CHILD_INTENDATION_PX);
		childOption->setPreferredWidth({{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {-CHILD_INTENDATION_PX, egui::RelativityMode::ABSOLUTE_VALUE}});

		updateDisabledStateOfChild(childOption);
		updatePreferredHeight();
	}
	void OptionGroup::updateDisabledStateOfChild(std::shared_ptr<BoolOptionElement> childOption) {
		updateDisabledStateOfChild(childOption, rootOption->isToggled());
	}
	void OptionGroup::updateDisabledStateOfChild(std::shared_ptr<BoolOptionElement> childOption, bool isToggled) {
		bool shouldDisable = shouldDisableChildFunc(isToggled);
		childOption->setVisible(! shouldDisable);
	}
	void OptionGroup::updatePreferredHeight() {
		egui::Value height((children.size() - 1) * SPACE_BETWEEN_ELEMS_PX, egui::RelativityMode::ABSOLUTE_VALUE);
		height.add(rootOption->getPreferredHeight());
		for(auto& c : children) {
			if(c->isVisible()) height.add(c->getPreferredHeight());
		}
		setPreferredHeight(height);
	}
}