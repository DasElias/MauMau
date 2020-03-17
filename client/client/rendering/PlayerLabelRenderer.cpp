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
	float const PlayerLabelRenderer::PADDING_LEFT_RIGHT = 0.0575f;
	float const PlayerLabelRenderer::PADDING_TOP = 0.1725f;
	glm::vec4 const PlayerLabelRenderer::CIRCLE_SECTOR_COLOR = {0.93f, 0.62f, 0.16f, 0.5f};

	glm::vec2 const PlayerLabelRenderer::LOCAL_PLAYER_POSITION = {0.25f, 0.65f};
	glm::vec2 const PlayerLabelRenderer::VIS_A_VIS_PLAYER_POSITION = {0.5f - (PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN / 2), 0};
	glm::vec2 const PlayerLabelRenderer::LEFT_PLAYER_POSITION = {PADDING_LEFT_RIGHT, PADDING_TOP};
	glm::vec2 const PlayerLabelRenderer::RIGHT_PLAYER_POSITION = {1 - PADDING_LEFT_RIGHT - PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN, PADDING_TOP};

	PlayerLabelRenderer::PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, AvatarTextures& avatarTextures, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer) :
		eguiRenderer(eguiRenderer),
		renderer2D(renderer2D),
		circleSectorRenderer(circleSectorRenderer),
		avatarTextures(avatarTextures),
		labelElementForLocal(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())),
		labelElementForVisAVis(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())),
		labelElementForLeft(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())),
		labelElementForRight(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio())) {

		auto parentElement = std::make_shared<egui::UnorganizedParentElement>();
		parentElement->addChildElement(labelElementForLocal);
		parentElement->addChildElement(labelElementForVisAVis);
		parentElement->addChildElement(labelElementForLeft);
		parentElement->addChildElement(labelElementForRight);
		scene.setRootElement(parentElement);

		labelElementForLocal->setPositionOnScreen(LOCAL_PLAYER_POSITION);
		labelElementForVisAVis->setPositionOnScreen(VIS_A_VIS_PLAYER_POSITION);
		labelElementForLeft->setPositionOnScreen(LEFT_PLAYER_POSITION);
		labelElementForRight->setPositionOnScreen(RIGHT_PLAYER_POSITION);
	}
	void PlayerLabelRenderer::renderLocal(const std::shared_ptr<ProxyPlayer>& participant) {
		renderImpl(participant, labelElementForLocal, playerLocal);
	}
	void PlayerLabelRenderer::renderVisAVis(const std::shared_ptr<ProxyPlayer>& participant) {
		renderImpl(participant, labelElementForVisAVis, playerVisAVis);
	}
	void PlayerLabelRenderer::renderLeft(const std::shared_ptr<ProxyPlayer>& participant) {
		renderImpl(participant, labelElementForLeft, playerLeft);
	}
	void PlayerLabelRenderer::renderRight(const std::shared_ptr<ProxyPlayer>& participant) {
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

		float const CIRCLE_RADIUS = 117 / 2.0f;
		float const CIRCLE_DIAMETER = 2 * CIRCLE_RADIUS;
		float const CIRCLE_CENTER_OFFSET_PX_X = 5 + (CIRCLE_RADIUS);
		float const CIRCLE_CENTER_OFFSET_PX_Y = 19 + (CIRCLE_RADIUS);
		centerX += playerLabel->getImageElement()->getComputedWidth() / avatarTextures.getWidth() * CIRCLE_CENTER_OFFSET_PX_X;
		centerY += playerLabel->getImageElement()->getComputedHeight() / avatarTextures.getHeight() * CIRCLE_CENTER_OFFSET_PX_Y;

		float diameterX = playerLabel->getImageElement()->getComputedWidth() / avatarTextures.getWidth() * (CIRCLE_DIAMETER + 25);
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