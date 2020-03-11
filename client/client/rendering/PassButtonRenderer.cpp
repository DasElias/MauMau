#include "PassButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <res/ingame/maubutton.png.h>

namespace card {
	float const PassButtonRenderer::BTN_WIDTH_PERCENT = 0.075f;

	PassButtonRenderer::PassButtonRenderer(egui::MasterRenderer& eguiRenderer, std::function<void(void)> passFunction) :
			eguiRenderer(eguiRenderer) {

		passBtn = std::make_shared<IngameButton>(
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_maubutton, tex_maubutton_size)),
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_maubuttondisabled, tex_maubuttondisabled_size)),
			BTN_WIDTH_PERCENT,
			passFunction
		);
		passBtn->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.775f, 0.6f));
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
}