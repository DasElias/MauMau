#include "PlayerLabelRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../utils/FileUtils.h"

namespace card {
	PlayerLabelRenderer::PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D) :
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			playerVisAVis(std::make_shared<PlayerLabel>()),
			playerLeft(std::make_shared<PlayerLabel>()),
			playerRight(std::make_shared<PlayerLabel>()),
			textureVisAVis(getApplicationFolder() + "\\resources\\user.png"),
			textureLeft(textureVisAVis),
			textureRight(textureVisAVis) {

		auto parentElement = std::make_shared<egui::UnorganizedParentElement>();
		parentElement->addChildElement(playerVisAVis);
		parentElement->addChildElement(playerLeft);
		parentElement->addChildElement(playerRight);
		scene.setRootElement(parentElement);

		updatePositions();
	}
	void PlayerLabelRenderer::renderVisAVis(const std::shared_ptr<ParticipantOnClient>& participant) {
		playerVisAVis->set(participant->getUsername());
	}
	void PlayerLabelRenderer::renderLeft(const std::shared_ptr<ParticipantOnClient>& participant) {
		playerLeft->set(participant->getUsername());
	}
	void PlayerLabelRenderer::renderRight(const std::shared_ptr<ParticipantOnClient>& participant) {
		playerRight->set(participant->getUsername());
	}
	void PlayerLabelRenderer::flush() {
		flushText();
		flushImages();
		endFlush();
	}
	void PlayerLabelRenderer::updatePositions() {
		float const PADDING_LEFT_RIGHT = 0.055f;
		float const PADDING_TOP = 0.175f;

		playerVisAVis->setPositionOnScreen(0.5f - (PlayerLabel::WIDTH_RELATIVE_ON_SCREEN/2), 0);
		playerLeft->setPositionOnScreen(PADDING_LEFT_RIGHT, PADDING_TOP);
		playerRight->setPositionOnScreen(1 - PADDING_LEFT_RIGHT - PlayerLabel::WIDTH_RELATIVE_ON_SCREEN, PADDING_TOP);
	}
	void PlayerLabelRenderer::flushText() {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void PlayerLabelRenderer::flushImages() {
		glActiveTexture(GL_TEXTURE0);
		if(playerVisAVis->isVisible()) {
			glBindTexture(GL_TEXTURE_2D, textureVisAVis.getTexId());
			renderer2D.render(playerVisAVis->getImageElement());
		}
		if(playerLeft->isVisible()) {
			glBindTexture(GL_TEXTURE_2D, textureLeft.getTexId());
			renderer2D.render(playerLeft->getImageElement());
		}
		if(playerRight->isVisible()) {
			glBindTexture(GL_TEXTURE_2D, textureRight.getTexId());
			renderer2D.render(playerRight->getImageElement());
		}
	}
	void PlayerLabelRenderer::endFlush() {
		playerVisAVis->clear();
		playerLeft->clear();
		playerRight->clear();
	}
}