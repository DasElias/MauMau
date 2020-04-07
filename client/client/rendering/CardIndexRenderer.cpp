#include "CardIndexRenderer.h"
#include "CardIndexRenderer.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include "../utils/FileUtils.h"
#include "../utils/DimensionsConversionUtils.h"
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <res/ingame/nextCardIndex.png.h>
#include "CardStackPositions.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include "CardRenderer.h"
#include <cmath>

namespace card {
	const int CardIndexRenderer::SPACE_BETWEEN_TITLE_AND_IMAGE_PX = 5;
	const int CardIndexRenderer::TITLE_STRETCH = 25;

	CardIndexRenderer::CardIndexRenderer(Renderer2D& renderer2D, CardIndexTextures& cardIndexTextures, ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			renderer2D(renderer2D),
			worldToScreenConverter(projectionMatrix, viewport),
			cardIndexTextures(cardIndexTextures),
			letteringTexture(
				SimpleTextureFactory()
				.setMagFilter(TextureMagFilter::NEAREST)
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR)
				.loadFromMemory(tex_nextCardIndex, tex_nextCardIndex_size)
			){

		parentElement = std::make_shared<egui::VBox>();
		parentElement->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1, 1)));
		title = std::make_shared<egui::AspectRatioElement>(letteringTexture.getAspectRatio());
		imageElement = std::make_shared<egui::AspectRatioElement>(cardIndexTextures.getAspectRatio());
		parentElement->addChildElement(title);		
		parentElement->addChildElement(imageElement);
		scene.setRootElement(parentElement);

		parentElementPositioning = std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.44f);
		parentElement->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(
			parentElementPositioning
		));
		parentElement->setPreferredWidth({0.05f, egui::RelativityMode::RELATIVE_ON_SCREEN});
		parentElement->setSpaceBetweenElements({SPACE_BETWEEN_TITLE_AND_IMAGE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		title->setMaxWidth({{1, egui::RelativityMode::RELATIVE_IN_PARENT}, {TITLE_STRETCH, egui::RelativityMode::ABSOLUTE_VALUE}});
		title->setXTranslation(-TITLE_STRETCH/2);
	/*	float const TITLE_STRETCH = 15;
		title->setXTranslation(-TITLE_STRETCH/2);
		title->setStretchX(TITLE_STRETCH);*/
		imageElement->setMaxWidth({1, egui::RelativityMode::RELATIVE_IN_PARENT});
	}
	void CardIndexRenderer::renderCardIndexForNextCard(CardIndex indexToRender) {
		if(indexToRender == CardIndex::NULLINDEX) return;

		updateParentElementPositioning();	
		updateParentElementWidth();

		glDisable(GL_DEPTH_TEST);
		cardIndexTextures.getTexture(indexToRender).bind();
		renderer2D.render(imageElement, true);

		letteringTexture.bind();
		renderer2D.render(title, true);
		glEnable(GL_DEPTH_TEST);
	}
	void CardIndexRenderer::updateParentElementPositioning() {
		glm::vec3 position3d = DRAW_CARDS_POSITION - glm::vec3(0, 0, CardRenderer::HEIGHT * 0.33f);
		float drawCardStack_yOnScreenPercent = worldToScreenConverter.convertWorldToScreen_percent(position3d).y;
		drawCardStack_yOnScreenPercent -= title->getComputedHeight() / 2;
		drawCardStack_yOnScreenPercent -= imageElement->getComputedHeight() / 2;
		drawCardStack_yOnScreenPercent -= egui::y_pixelToPercent(SPACE_BETWEEN_TITLE_AND_IMAGE_PX) / 2;
		parentElementPositioning->setY(drawCardStack_yOnScreenPercent);
	}
	void CardIndexRenderer::updateParentElementWidth() {
		float distanceBetweenCardStacks = getDistanceBetweenCardStacks();
		distanceBetweenCardStacks -= egui::x_pixelToPercent(TITLE_STRETCH);
		distanceBetweenCardStacks *= 0.65f;

		distanceBetweenCardStacks = std::max<float>(egui::x_pixelToPercent(1), distanceBetweenCardStacks);

		parentElement->setPreferredWidth({distanceBetweenCardStacks, egui::RelativityMode::RELATIVE_ON_SCREEN});
	}
	float CardIndexRenderer::getDistanceBetweenCardStacks() {
		glm::vec3 drawCardsPosition = DRAW_CARDS_POSITION + glm::vec3(CardRenderer::WIDTH / 2, 0, 0);
		glm::vec3 playCardsPosition = PLAY_CARDS_POSITION - glm::vec3(CardRenderer::WIDTH / 2, 0, 0);
		float drawCardsX = worldToScreenConverter.convertWorldToScreen_percent(drawCardsPosition).x;
		float playCardsX = worldToScreenConverter.convertWorldToScreen_percent(playCardsPosition).x;
		return abs(playCardsX - drawCardsX);
	}
}