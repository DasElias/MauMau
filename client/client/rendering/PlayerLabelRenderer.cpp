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
	PlayerLabelRenderer::PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer) :
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			circleSectorRenderer(circleSectorRenderer),
			textureSkip(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromFile(getApplicationFolder() + "\\resources\\skipPlayer.png")),
			textureMau(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromFile("C:\\Users\\Elias\\Downloads\\Bild5.png")),
			labelElementForLocal(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio(), textureSkip.getAspectRatio(), textureMau.getAspectRatio())),
			labelElementForVisAVis(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio(), textureSkip.getAspectRatio(), textureMau.getAspectRatio())),
			labelElementForLeft(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio(), textureSkip.getAspectRatio(), textureMau.getAspectRatio())),
			labelElementForRight(std::make_shared<PlayerLabel>(avatarTextures.getAspectRatio(), textureSkip.getAspectRatio(), textureMau.getAspectRatio())) {

		auto parentElement = std::make_shared<egui::UnorganizedParentElement>();
		parentElement->addChildElement(labelElementForLocal);
		parentElement->addChildElement(labelElementForVisAVis);
		parentElement->addChildElement(labelElementForLeft);
		parentElement->addChildElement(labelElementForRight);
		scene.setRootElement(parentElement);

		updatePositions();
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
	void PlayerLabelRenderer::renderImpl(const std::shared_ptr<ProxyPlayer>& participant, std::shared_ptr<PlayerLabel>& labelElementField, std::shared_ptr<ProxyPlayer>& proxyPlayerField) {
		proxyPlayerField = participant;

		std::optional<float> animationToUse = participant->getPercentOfSkipAnimationOrNone();
		if(! animationToUse.has_value()) animationToUse = participant->getPercentOfMauAnimationOrNone();
		labelElementField->set(participant->getUsername(), participant->getPercentOfSkipAnimationOrNone(), participant->getPercentOfMauAnimationOrNone());

		if(participant->isRemainingTimeAnimationActive()) renderCircleSector(labelElementField, *participant->getPercentOfRemainingTime());
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
		float const PADDING_LEFT_RIGHT = 0.0575f;
		float const PADDING_TOP = 0.1725f;

		labelElementForLocal->setPositionOnScreen(0.25f, 0.65f);
		labelElementForVisAVis->setPositionOnScreen(0.5f - (PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN/2), 0);
		labelElementForLeft->setPositionOnScreen(PADDING_LEFT_RIGHT, PADDING_TOP);
		labelElementForRight->setPositionOnScreen(1 - PADDING_LEFT_RIGHT - PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN, PADDING_TOP);
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
	void PlayerLabelRenderer::flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, const std::shared_ptr<ProxyPlayer>& participant) {
		if(element->isVisible()) {
			Avatar avatar = participant->getAvatar();
			avatarTextures.bind(avatar);
			renderer2D.render(element->getImageElement(), true);

			
			auto skipElementOrNull = element->getAnimationOverlayElementIfVisible();
			if(skipElementOrNull) {
				textureSkip.bind();
				renderer2D.render(skipElementOrNull, true);
			}
			auto mauElementOrNull = element->getMauAnimationElementIfVisible();
			if(mauElementOrNull) {
				textureMau.bind();
				renderer2D.render(mauElementOrNull, true);
			}
		}
	}
	void PlayerLabelRenderer::endFlush() {
		labelElementForLocal->clear();
		labelElementForLeft->clear();
		labelElementForRight->clear();
		labelElementForVisAVis->clear();
	}
}