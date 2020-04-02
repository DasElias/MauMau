#include "PassButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodeComponents/background/DisabledBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <res/ingame/passbutton.png.h>

namespace card {
	float const PassButtonRenderer::BTN_WIDTH_PERCENT = 0.0875f;

	PassButtonRenderer::PassButtonRenderer(egui::MasterRenderer& eguiRenderer) :
			eguiRenderer(eguiRenderer) {

		passBtn = std::make_shared<IngameButton>(
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_passbutton, tex_passbutton_size)),
			std::make_shared<egui::DisabledBackground>(),
			BTN_WIDTH_PERCENT
		);
		passBtn->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.775f, 0.65f));
		scene.setRootElement(passBtn);
	}
	void PassButtonRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void PassButtonRenderer::render() {		
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void PassButtonRenderer::addClickHandler(std::function<void(void)> passFunction) {
		passBtn->addClickHandler(passFunction);
	}
}