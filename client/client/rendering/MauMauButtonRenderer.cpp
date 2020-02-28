#include "MauMauButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <res/ingame/maubutton.png.h>

namespace card {
	float const MauMauButtonRenderer::MAU_MAU_BTN_WIDTH_PERCENT = 0.075f;

	MauMauButtonRenderer::MauMauButtonRenderer(egui::MasterRenderer& eguiRenderer, std::function<void(void)> mauMauFunction) :
			eguiRenderer(eguiRenderer) {

		mauMauButtonBgElement = std::make_shared<egui::AspectRatioElement>(1.0f);
		mauMauButtonBgElement->setRadius(0.5f, true);
		mauMauButtonBgElement->setMaxWidth({MAU_MAU_BTN_WIDTH_PERCENT, egui::RelativityMode::RELATIVE_ON_SCREEN});
		mauMauButtonBgElement->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.7f, 0.6f));
		mauMauButtonBgElement->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_maubutton, tex_maubutton_size)
		));
		mauMauButtonBgElement->setDisabledBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_maubuttondisabled, tex_maubuttondisabled_size)
		), egui::RenderMode::RENDER_EXCLUSIVELY);


		mauMauButtonClickable = std::make_shared<egui::AspectRatioElement>(1.0f);
		mauMauButtonClickable->setRadius(0.5f, true);
		float buttonWidth_withoutShadow = 300/float(330);
		mauMauButtonClickable->setMaxWidth({MAU_MAU_BTN_WIDTH_PERCENT * buttonWidth_withoutShadow, egui::RelativityMode::RELATIVE_ON_SCREEN});
		mauMauButtonClickable->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.7f, 0.6f));
		mauMauButtonClickable->getHoverEventManager().addEventHandler({[this](egui::HoverEvent& evt) {
			int const SIZE_ADDITION_PX = 20;

			mauMauButtonBgElement->setXTranslation(evt.isHovered() ? -SIZE_ADDITION_PX / 2 : 0);
			mauMauButtonBgElement->setYTranslation(evt.isHovered() ? -SIZE_ADDITION_PX / 2 : 0);

			if(evt.isHovered()) {
				mauMauButtonBgElement->setMaxWidth({{MAU_MAU_BTN_WIDTH_PERCENT, egui::RelativityMode::RELATIVE_ON_SCREEN}, {SIZE_ADDITION_PX, egui::RelativityMode::ABSOLUTE_VALUE}});
			} else {
				mauMauButtonBgElement->setMaxWidth({MAU_MAU_BTN_WIDTH_PERCENT, egui::RelativityMode::RELATIVE_ON_SCREEN});
			}
		}});
		mauMauButtonClickable->getMouseClickedEventManager().addEventHandler({[this, mauMauFunction](egui::MouseEvent&) {
			mauMauFunction();
		}});

		std::shared_ptr<egui::UnorganizedParentElement> parent(new egui::UnorganizedParentElement({mauMauButtonBgElement, mauMauButtonClickable}));
		scene.setRootElement(parent);
	}
	void MauMauButtonRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void MauMauButtonRenderer::render(bool canMau) {
		mauMauButtonBgElement->setDisabled(! canMau);
		
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
}