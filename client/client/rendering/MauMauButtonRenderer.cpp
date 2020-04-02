#include "MauMauButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <res/ingame/maubutton.png.h>

namespace card {
	float const MauMauButtonRenderer::BTN_WIDTH_PERCENT = 0.075f;

	MauMauButtonRenderer::MauMauButtonRenderer(egui::MasterRenderer& eguiRenderer) :
			eguiRenderer(eguiRenderer) {

		mauMauBtn = std::make_shared<IngameButton>(
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_maubutton, tex_maubutton_size)),
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_maubuttondisabled, tex_maubuttondisabled_size)),
			BTN_WIDTH_PERCENT
		);
		mauMauBtn->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.7f, 0.6f));
		scene.setRootElement(mauMauBtn);
	}
	void MauMauButtonRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void MauMauButtonRenderer::render(bool canMau) {
		mauMauBtn->setDisabled(! canMau);
		
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void MauMauButtonRenderer::addClickHandler(std::function<void(void)> mauMauFunction) {
		mauMauBtn->addClickHandler(mauMauFunction);
	}
}