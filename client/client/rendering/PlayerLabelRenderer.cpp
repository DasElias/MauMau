#include "PlayerLabelRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../utils/FileUtils.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include <shared\utils\MathUtils.h>
#include <shared\utils\TimeUtils.h>
#include <shared/model/MaxTurnDuration.h>
#include <egui/input/IOHandler.h>

namespace card {
	PlayerLabelRenderer::PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer) :
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			circleSectorRenderer(circleSectorRenderer),
			textureSkip(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromFile(getApplicationFolder() + "\\resources\\skipPlayer.png")),
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

		if(participant->isRemainingTimeAnimationActive()) renderCircleSector(playerLocal, *participant->getPercentOfRemainingTime());
	}
	void PlayerLabelRenderer::renderVisAVis(const std::shared_ptr<ProxyPlayer>& participant) {
		playerVisAVis->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		visAVisAvatar = participant->getAvatar();

		if(participant->isRemainingTimeAnimationActive()) renderCircleSector(playerVisAVis, *participant->getPercentOfRemainingTime());
	}
	void PlayerLabelRenderer::renderLeft(const std::shared_ptr<ProxyPlayer>& participant) {
		playerLeft->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		leftAvatar = participant->getAvatar();

		if(participant->isRemainingTimeAnimationActive()) renderCircleSector(playerLeft, *participant->getPercentOfRemainingTime());
	}
	void PlayerLabelRenderer::renderRight(const std::shared_ptr<ProxyPlayer>& participant) {
		playerRight->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone());
		rightAvatar = participant->getAvatar();
		
		if(participant->isRemainingTimeAnimationActive()) renderCircleSector(playerRight, *participant->getPercentOfRemainingTime());
	}
	void PlayerLabelRenderer::flush() {
		flushText();
		flushImages();
		endFlush();
	}
	void PlayerLabelRenderer::renderCircleSector(const std::shared_ptr<PlayerLabel>& playerLabel, float percentExpired) {
		float startAngle = PI/2;
		float endAngle = interpolateLinear(percentExpired, 0, PI/2 + 2*PI, 1, PI/2);

		float centerX = playerLabel->getImageElement()->getAbsXMargin();
		float centerY = playerLabel->getImageElement()->getAbsYMargin();
		
		float const CIRCLE_RADIUS = 117 / 2.0f;
		float const CIRCLE_DIAMETER = 2 * CIRCLE_RADIUS;
		float const CIRCLE_CENTER_OFFSET_PX_X = 5 + (CIRCLE_RADIUS);
		float const CIRCLE_CENTER_OFFSET_PX_Y = 19 + (CIRCLE_RADIUS);
		centerX += playerLabel->getImageElement()->getComputedWidth() / avatarTextures.getWidth() * CIRCLE_CENTER_OFFSET_PX_X;
		centerY += playerLabel->getImageElement()->getComputedHeight() / avatarTextures.getHeight() * CIRCLE_CENTER_OFFSET_PX_Y;

		float diameterX = playerLabel->getImageElement()->getComputedWidth() / avatarTextures.getWidth() * (CIRCLE_DIAMETER + 25);
		circleSectorRenderer.renderSector_xDiameter({centerX, centerY}, diameterX, startAngle, endAngle, 500, {0.93f, 0.62f, 0.16f, 0.5f});
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