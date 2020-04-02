#include "ToggleSwitch.h"
#include <egui/model/positioning/FunctionalPositioningInParent.h>
#include <egui/model/positioning/CenterYInParentWrapper.h>
#include <shared/utils/TimeUtils.h>
#include "../utils/VectorMathUtils.h"
#include <shared/utils/MathUtils.h>
#include <algorithm>
#include "ClickableCursorUtils.h"

namespace card {
	float const ToggleSwitch::SWITCH_ELEM_PADDING = 0.025f;
	float const ToggleSwitch::SWTICH_ELEM_WIDTH = (1 - 2*ToggleSwitch::SWITCH_ELEM_PADDING) / 2;

	ToggleSwitch::ToggleSwitch() :
			unixToggleTimeMs(getMilliseconds() - ANIMATION_DURATION_MS),
			isToggled_field(false) {

		switchElement = std::make_shared<egui::UnorganizedParentElement>();
		addChildElement(switchElement);
		switchElement->setOwnPositioning(std::make_shared<egui::CenterYInParentWrapper>(
			std::make_shared<egui::FunctionalPositioningInParent>(
				[this](PositionableElement& target) {
					return getSwitchElementXMargin();
				},
				[](PositionableElement& target) {
					return 0;
				}
			)
		));
		switchElement->getAbsYMargin();
		switchElement->setPreferredWidth({SWTICH_ELEM_WIDTH, egui::RelativityMode::RELATIVE_IN_PARENT});
		switchElement->setPreferredHeight({0.8f, egui::RelativityMode::RELATIVE_IN_PARENT});
		switchElement->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1, 1, 1)));

		background = std::make_shared<egui::ColoredBackground>(egui::Color(0.8f, 0.8f, 0.8f));
		setBackground(background);
		setDisabledBackground(std::make_shared<egui::ColoredBackground>(
			egui::Color(1, 1, 1, 0.7f)
		), egui::RENDER_ADDITIONALLY);
		getMousePressedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			updateToggleFieldAndFireEvent(!isToggled_field);
			unixToggleTimeMs = getMilliseconds();
		}});	

		setCursorToHandOnHover(*this);
	}
	
	float ToggleSwitch::getSwitchElementXMargin() {
		float x = float(getMilliseconds() - unixToggleTimeMs);
		float x1 = (isToggled_field ? 0 : ANIMATION_DURATION_MS);
		float x2 = (isToggled_field ? ANIMATION_DURATION_MS : 0);

		float const startX = SWITCH_ELEM_PADDING;
		float const endX = 1 - SWTICH_ELEM_WIDTH - SWITCH_ELEM_PADDING;

		float margin = interpolateLinear(x, x1, startX, x2, endX);
		return std::clamp(margin, startX, endX);
	}
	bool ToggleSwitch::isToggled() const {
		return isToggled_field;
	}
	void ToggleSwitch::set(bool isToggled) {
		updateToggleFieldAndFireEvent(isToggled);
		unixToggleTimeMs = (getMilliseconds() - ANIMATION_DURATION_MS);
	}
	void ToggleSwitch::toggle() {
		updateToggleFieldAndFireEvent(!isToggled_field);
		unixToggleTimeMs = (getMilliseconds() - ANIMATION_DURATION_MS);
	}
	void ToggleSwitch::toggleWithAnimation() {
		updateToggleFieldAndFireEvent(!isToggled_field);
		unixToggleTimeMs = getMilliseconds();
	}
	void ToggleSwitch::renderGuiComponents(egui::MasterRenderer& renderer) {
		glm::vec3 const offColor = {0.8, 0.8, 0.8};
		glm::vec3 const onColor = {0.13, 0.59, 0.95};

		float x = float(getMilliseconds() - unixToggleTimeMs);
		float x1 = (isToggled_field ? 0 : ANIMATION_DURATION_MS);
		float x2 = (isToggled_field ? ANIMATION_DURATION_MS : 0);
		glm::vec3 color = interpolateLinear(x, x1, offColor, x2, onColor);
		color = clamp(color, offColor, onColor);

		background->setBackgroundColor(egui::Color(color.x, color.y, color.z));

		Node::renderGuiComponents(renderer);
	}
	void ToggleSwitch::updateToggleFieldAndFireEvent(bool newState) {
		isToggled_field = newState;
		egui::ToggleEvent evt(newState);
		toggleEventManager.fireEvent(evt);
	}
	egui::EventManager<egui::ToggleEvent>& ToggleSwitch::getToggleEventManager() {
		return toggleEventManager;
	}
}