#include "MauMauButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>

namespace card {
	float const MauMauButtonRenderer::MAU_MAU_BTN_WIDTH_PERCENT = 0.075f;

	MauMauButtonRenderer::MauMauButtonRenderer(egui::MasterRenderer& eguiRenderer, std::function<void(void)> mauMauFunction) :
			eguiRenderer(eguiRenderer) {

		mauMauButton = std::make_shared<egui::AspectRatioElement>(1.0f);
		mauMauButton->setRadius(0.5f, true);
		mauMauButton->setMaxWidth({MAU_MAU_BTN_WIDTH_PERCENT, egui::RelativityMode::RELATIVE_ON_SCREEN});
		mauMauButton->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture("C:\\Users\\Elias\\Documents\\PROJECT X\\c.png")
		));
		mauMauButton->setDisabledBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture("C:\\Users\\Elias\\Documents\\PROJECT X\\d.png")
		), egui::RenderMode::RENDER_EXCLUSIVELY);
		mauMauButton->getHoverEventManager().addEventHandler({[this](egui::HoverEvent& evt) {
			int const SIZE_ADDITION_PX = 20;
		
			mauMauButton->setXTranslation(evt.isHovered() ? -SIZE_ADDITION_PX/2 : 0);
			mauMauButton->setYTranslation(evt.isHovered() ? -SIZE_ADDITION_PX / 2 : 0);
			
			if(evt.isHovered()) {
				mauMauButton->setMaxWidth({{MAU_MAU_BTN_WIDTH_PERCENT, egui::RelativityMode::RELATIVE_ON_SCREEN}, {SIZE_ADDITION_PX, egui::RelativityMode::ABSOLUTE_VALUE}});
			} else {
				mauMauButton->setMaxWidth({MAU_MAU_BTN_WIDTH_PERCENT, egui::RelativityMode::RELATIVE_ON_SCREEN});
			}
		}});
		mauMauButton->getMouseClickedEventManager().addEventHandler({[this, mauMauFunction](egui::MouseEvent&) {
			mauMauFunction();
		}});
		mauMauButton->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.7f, 0.6f));
		std::shared_ptr<egui::UnorganizedParentElement> parent(new egui::UnorganizedParentElement({mauMauButton}));
		scene.setRootElement(parent);
	}
	void MauMauButtonRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void MauMauButtonRenderer::render(bool canMau) {
		mauMauButton->setDisabled(! canMau);
		
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
}