#include "PassButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodeComponents/background/DisabledBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <res/ingame/passbutton.png.h>

namespace card {
	float const PassButtonRenderer::BTN_WIDTH_PERCENT = 0.0875f;

	PassButtonRenderer::PassButtonRenderer(egui::MasterRenderer& eguiRenderer, ProjectionMatrix& pm, Viewport& vp) :
			eguiRenderer(eguiRenderer),
			mauMauButtonPositionGenerator(pm, vp),
			passBtnPositioning(std::make_shared<egui::RelativePositioningOnScreen>()) {

		passBtn = std::make_shared<IngameButton>(
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_passbutton, tex_passbutton_size)),
			std::make_shared<egui::DisabledBackground>(),
			BTN_WIDTH_PERCENT
		);
		passBtn->setOwnPositioning(passBtnPositioning);
		scene.setRootElement(passBtn);
	}
	void PassButtonRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void PassButtonRenderer::render() {	
		updateBtnPosition();

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void PassButtonRenderer::addClickHandler(std::function<void(void)> passFunction) {
		passBtn->addClickHandler(passFunction);
	}
	void PassButtonRenderer::updateBtnPosition() {
		glm::vec2 positionOnScreen = mauMauButtonPositionGenerator.getPositionOnScreen();
		positionOnScreen += glm::vec2(0.075f, 0.045f);

		passBtnPositioning->setX(positionOnScreen.x);
		passBtnPositioning->setY(positionOnScreen.y);
	}
}