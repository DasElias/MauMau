#include "IngameButton.h"
#include "../gui/ClickableCursorUtils.h"

namespace card {
	IngameButton::IngameButton(std::shared_ptr<egui::Background> background, std::shared_ptr<egui::Background> disabledBackground, float relativeWidthOnScreen) {
		mauMauButtonBgElement = std::make_shared<egui::AspectRatioElement>(1.0f);
		mauMauButtonBgElement->setRadius(0.5f, true);
		mauMauButtonBgElement->setMaxWidth({relativeWidthOnScreen, egui::RelativityMode::RELATIVE_ON_SCREEN});
		mauMauButtonBgElement->setBackground(background);
		mauMauButtonBgElement->setDisabledBackground(disabledBackground, egui::RenderMode::RENDER_EXCLUSIVELY);


		mauMauButtonClickable = std::make_shared<egui::AspectRatioElement>(1.0f);
		mauMauButtonClickable->setRadius(0.5f, true);
		float buttonWidth_withoutShadow = 300 / float(330);
		mauMauButtonClickable->setMaxWidth({relativeWidthOnScreen * buttonWidth_withoutShadow, egui::RelativityMode::RELATIVE_ON_SCREEN});
		mauMauButtonClickable->getHoverEventManager().addEventHandler({[this, relativeWidthOnScreen](egui::HoverEvent& evt) {
			int const SIZE_ADDITION_PX = 20;

			mauMauButtonBgElement->setXTranslation(evt.isHovered() ? -SIZE_ADDITION_PX / 2 : 0);
			mauMauButtonBgElement->setYTranslation(evt.isHovered() ? -SIZE_ADDITION_PX / 2 : 0);

			if(evt.isHovered()) {
				mauMauButtonBgElement->setMaxWidth({{relativeWidthOnScreen, egui::RelativityMode::RELATIVE_ON_SCREEN}, {SIZE_ADDITION_PX, egui::RelativityMode::ABSOLUTE_VALUE}});
			} else {
				mauMauButtonBgElement->setMaxWidth({relativeWidthOnScreen, egui::RelativityMode::RELATIVE_ON_SCREEN});
			}
		}});

		addChildElement(mauMauButtonBgElement);
		addChildElement(mauMauButtonClickable);

		setCursorToHandOnHoverAndResetOnClick(*mauMauButtonClickable);
	}
	void IngameButton::setOwnPositioning(std::shared_ptr<egui::Positioning> positioning) {
		mauMauButtonBgElement->setOwnPositioning(positioning);
		mauMauButtonClickable->setOwnPositioning(positioning);
	}
	void IngameButton::addClickHandler(std::function<void(void)> handler) {
		mauMauButtonClickable->getMouseClickedEventManager().addEventHandler({[handler](egui::MouseEvent&) {
			handler();
		}});
	}
}