#include "MauMauButtonRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <res/ingame/maubutton.png.h>
#include "CardRenderer.h"
#include "CardStackPositions.h"
#include "../utils/VectorMathUtils.h"

namespace card {
	float const MauMauButtonRenderer::BTN_WIDTH_PERCENT = 0.075f;

	MauMauButtonRenderer::MauMauButtonRenderer(egui::MasterRenderer& eguiRenderer, ProjectionMatrix& pm, Viewport& vp) :
			eguiRenderer(eguiRenderer),
			mauMauPositionGenerator(pm, vp) {

		mauMauBtn = std::make_shared<IngameButton>(
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_maubutton, tex_maubutton_size)),
			std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_maubuttondisabled, tex_maubuttondisabled_size)),
			BTN_WIDTH_PERCENT
		);
		mauMauBtnPositioning = std::make_shared<egui::RelativePositioningOnScreen>();
		mauMauBtn->setOwnPositioning(mauMauBtnPositioning);
		scene.setRootElement(mauMauBtn);
	}
	void MauMauButtonRenderer::clearPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void MauMauButtonRenderer::render(bool canMau) {
		mauMauBtn->setDisabled(! canMau);

		updateBtnPosition();
		
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void MauMauButtonRenderer::addClickHandler(std::function<void(void)> mauMauFunction) {
		mauMauBtn->addClickHandler(mauMauFunction);
	}
	void MauMauButtonRenderer::updateBtnPosition() {
		glm::vec2 posOnScreen = mauMauPositionGenerator.getPositionOnScreen();
		mauMauBtnPositioning->setX(posOnScreen.x);
		mauMauBtnPositioning->setY(posOnScreen.y);
	}
}