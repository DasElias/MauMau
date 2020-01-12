#include "PlayerLabelRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../utils/FileUtils.h"
#include "../renderingModel/SimpleTextureFactory.h"

namespace card {
	PlayerLabelRenderer::PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D) :
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			textureSkip(SimpleTextureFactory(getApplicationFolder() + "\\resources\\skipPlayer.png").setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).generateTexture()),
			playerLocal(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())),
			playerVisAVis(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())),
			playerLeft(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())),
			playerRight(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())) {

		auto parentElement = std::make_shared<egui::UnorganizedParentElement>();
		parentElement->addChildElement(playerLocal);
		parentElement->addChildElement(playerVisAVis);
		parentElement->addChildElement(playerLeft);
		parentElement->addChildElement(playerRight);
		scene.setRootElement(parentElement);

		updatePositions();
	}
	void PlayerLabelRenderer::renderLocal(const std::shared_ptr<ProxyPlayer>& participant) {
		playerLocal->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		localAvatar = participant->getAvatar();
	}
	void PlayerLabelRenderer::renderVisAVis(const std::shared_ptr<ProxyPlayer>& participant) {
		playerVisAVis->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		visAVisAvatar = participant->getAvatar();
	}
	void PlayerLabelRenderer::renderLeft(const std::shared_ptr<ProxyPlayer>& participant) {
		playerLeft->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		leftAvatar = participant->getAvatar();
	}
	void PlayerLabelRenderer::renderRight(const std::shared_ptr<ProxyPlayer>& participant) {
		playerRight->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		rightAvatar = participant->getAvatar();
	}
	void PlayerLabelRenderer::flush() {
		flushText();
		flushImages();
		endFlush();
	}
	void PlayerLabelRenderer::updatePositions() {
		float const PADDING_LEFT_RIGHT = 0.055f;
		float const PADDING_TOP = 0.175f;

		playerLocal->setPositionOnScreen(0.25f, 0.65f);
		playerVisAVis->setPositionOnScreen(0.5f - (PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN/2), 0);
		playerLeft->setPositionOnScreen(PADDING_LEFT_RIGHT, PADDING_TOP);
		playerRight->setPositionOnScreen(1 - PADDING_LEFT_RIGHT - PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN, PADDING_TOP);
		auto imgElem = playerLocal->getImageElement();
		imgElem->getAbsXMargin();
	}
	void PlayerLabelRenderer::flushText() {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void PlayerLabelRenderer::flushImages() {
		glActiveTexture(GL_TEXTURE0);

		flushImageOfPlayer(playerLocal, localAvatar);
		flushImageOfPlayer(playerVisAVis, visAVisAvatar);
		flushImageOfPlayer(playerLeft, leftAvatar);
		flushImageOfPlayer(playerRight, rightAvatar);
	}
	void PlayerLabelRenderer::flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, Avatar avatar) {
		if(element->isVisible()) {
			avatarTextures.bind(avatar);
			renderer2D.render(element->getImageElement(), true);

			auto skipElementOrNull = element->getSkipElementIfVisible();
			if(skipElementOrNull) {
				textureSkip.bind();
				renderer2D.render(skipElementOrNull, true);
			}
		}
	}
	void PlayerLabelRenderer::endFlush() {
		playerVisAVis->clear();
		playerLeft->clear();
		playerRight->clear();
	}
}