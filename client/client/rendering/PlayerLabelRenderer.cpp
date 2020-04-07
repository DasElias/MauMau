#include "PlayerLabelRenderer.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../utils/FileUtils.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include <shared\utils\MathUtils.h>
#include <shared\utils\TimeUtils.h>
#include <shared/model/MaxTurnDuration.h>
#include <egui/input/IOHandler.h>
#include <shared/utils/Logger.h>

namespace card {
	int const PlayerLabelRenderer::CIRCLE_DIAMETER = 110;
	int const PlayerLabelRenderer::CIRCLE_RADIUS = PlayerLabelRenderer::CIRCLE_DIAMETER / 2.0f;
	int const PlayerLabelRenderer::CIRCLE_CENTER_OFFSET_PX_X = 9 + PlayerLabelRenderer::CIRCLE_RADIUS;
	int const PlayerLabelRenderer::CIRCLE_CENTER_OFFSET_PX_Y = 18 + PlayerLabelRenderer::CIRCLE_RADIUS;
	glm::vec4 const PlayerLabelRenderer::CIRCLE_SECTOR_COLOR = {0.93f, 0.62f, 0.16f, 0.5f};

	PlayerLabelRenderer::PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, AvatarTextures& avatarTextures, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer) :
		eguiRenderer(eguiRenderer),
		renderer2D(renderer2D),
		circleSectorRenderer(circleSectorRenderer),
		avatarTextures(avatarTextures),
		labelElementForLocal(std::make_shared<PlayerLabel>()),
		labelElementForVisAVis(std::make_shared<PlayerLabel>()),
		labelElementForLeft(std::make_shared<PlayerLabel>()),
		labelElementForRight(std::make_shared<PlayerLabel>()) {

		auto parentElement = std::make_shared<egui::UnorganizedParentElement>();
		parentElement->addChildElement(labelElementForLocal);
		parentElement->addChildElement(labelElementForVisAVis);
		parentElement->addChildElement(labelElementForLeft);
		parentElement->addChildElement(labelElementForRight);
		scene.setRootElement(parentElement);
	}
	void PlayerLabelRenderer::renderLocal(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen) {
		labelElementForLocal->setPositionOnScreen(positionOnScreen);
		renderImpl(participant, labelElementForLocal, playerLocal);
	}
	void PlayerLabelRenderer::renderVisAVis(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen) {
		labelElementForVisAVis->setPositionOnScreen(positionOnScreen);
		renderImpl(participant, labelElementForVisAVis, playerVisAVis);
	}
	void PlayerLabelRenderer::renderLeft(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen) {
		labelElementForLeft->setPositionOnScreen(positionOnScreen);
		renderImpl(participant, labelElementForLeft, playerLeft);
	}
	void PlayerLabelRenderer::renderRight(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen) {
		labelElementForRight->setPositionOnScreen(positionOnScreen);
		renderImpl(participant, labelElementForRight, playerRight);
	}
	void PlayerLabelRenderer::renderImpl(const std::shared_ptr<ProxyPlayer>& participant, std::shared_ptr<PlayerLabel>& labelElementField, std::weak_ptr<ProxyPlayer>& proxyPlayerField) {
		proxyPlayerField = participant;
		labelElementField->set(participant->getUsername());

		if(participant->isRemainingTimeAnimationActive()) renderCircleSector(labelElementField, *participant->getPercentOfRemainingTime());
	}
	void PlayerLabelRenderer::flush() {
		flushText();
		flushImages();
		endFlush();
	}
	void PlayerLabelRenderer::renderCircleSector(const std::shared_ptr<PlayerLabel>& playerLabel, float percentExpired) {
		float startAngle = PI / 2;
		float endAngle = interpolateLinear(percentExpired, 0, PI / 2 + 2 * PI, 1, PI / 2);

		float centerX = playerLabel->getImageElement()->getAbsXMargin();
		float centerY = playerLabel->getImageElement()->getAbsYMargin();

		centerX += playerLabel->getImageElement()->getComputedWidth() / avatarTextures.getWidth() * CIRCLE_CENTER_OFFSET_PX_X;
		centerY += playerLabel->getImageElement()->getComputedHeight() / avatarTextures.getHeight() * CIRCLE_CENTER_OFFSET_PX_Y;

		float diameterX = playerLabel->getImageElement()->getComputedWidth() / avatarTextures.getWidth() * (CIRCLE_DIAMETER + 20);
		circleSectorRenderer.renderSector_xDiameter({centerX, centerY}, diameterX, startAngle, endAngle, 500, CIRCLE_SECTOR_COLOR);
	}
	void PlayerLabelRenderer::flushText() {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void PlayerLabelRenderer::flushImages() {
		glActiveTexture(GL_TEXTURE0);
		flushImageOfPlayer(labelElementForLocal, playerLocal);
		flushImageOfPlayer(labelElementForVisAVis, playerVisAVis);
		flushImageOfPlayer(labelElementForLeft, playerLeft);
		flushImageOfPlayer(labelElementForRight, playerRight);
	}
	void PlayerLabelRenderer::flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, std::weak_ptr<ProxyPlayer> participant_wp) {
		if(element->isVisible()) {
			auto participant = participant_wp.lock();
			Avatar avatar = participant->getAvatar();
			avatarTextures.bind(avatar);
			renderer2D.render(element->getImageElement(), true);
		}
	}
	void PlayerLabelRenderer::endFlush() {
		labelElementForLocal->clear();
		labelElementForLeft->clear();
		labelElementForRight->clear();
		labelElementForVisAVis->clear();
	}
}