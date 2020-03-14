#include "MauMauPlayerLabelOverlayRenderer.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include "../utils/FileUtils.h"
#include <shared/utils/MathUtils.h>
#include "../gui/CombinedPositioning.h"
#include <res/ingame/mauspeechbubble.h>
#include <res/ingame/skipPlayer.png.h>
#include <egui/input/IOHandler.h>
#include <iostream>

namespace card {
	MauMauPlayerLabelOverlayRenderer::MauMauPlayerLabelOverlayRenderer(Renderer2D& renderer2D, float playerLabelWidthRelativeOnScreen, float avatarAspectRatio) :
			renderer2D(renderer2D),
			playerLabelWidthRelativeOnScreen(playerLabelWidthRelativeOnScreen),
			avatarAspectRatio(avatarAspectRatio),
			textureSkip(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromMemory(tex_skipPlayer, tex_skipPlayer_size)),
			textureMau(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromMemory(tex_mauspeechbubble, tex_mauspeechbubble_size)),
			skipAnimElement(std::make_shared<egui::AspectRatioElement>(textureSkip.getAspectRatio())),
			mauAnimElement(std::make_shared<egui::AspectRatioElement>(textureMau.getAspectRatio())),
			basicPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.0f)) {
			
		skipAnimElement->setOwnPositioning(basicPositioning);

		auto mauAnimElementPositioning = std::make_shared<CombinedPositioning>();
		mauAnimElementPositioning->addPart(basicPositioning);
		mauAnimElementPositioning->addPart(std::make_shared<egui::RelativePositioningOnScreen>(
			playerLabelWidthRelativeOnScreen * 0.55f, 0.0075f
		));
		mauAnimElement->setOwnPositioning(mauAnimElementPositioning);
	}
	void MauMauPlayerLabelOverlayRenderer::render(glm::vec2 positionRelativeOnScreen, std::optional<float> percentSkipAnimOrNone, bool renderSkipGrey, std::optional<float> percentMauAnimOrNone) {
		basicPositioning->setX(positionRelativeOnScreen.x);
		basicPositioning->setY(positionRelativeOnScreen.y);

		if(renderSkipGrey && !percentSkipAnimOrNone.has_value()) percentSkipAnimOrNone = 0;

		updateSkipElement(percentSkipAnimOrNone);
		updateMauElement(percentMauAnimOrNone);

		if(percentSkipAnimOrNone.has_value()) {
			textureSkip.bind();
			renderer2D.render(skipAnimElement, true);
		}
		if(percentMauAnimOrNone.has_value()) {
			textureMau.bind();
			renderer2D.render(mauAnimElement, true);
		}
	}
	void MauMauPlayerLabelOverlayRenderer::updateSkipElement(std::optional<float> percentSkipAnimOrNone) {
		bool isAnimationActive = percentSkipAnimOrNone.has_value();
		skipAnimElement->setVisible(isAnimationActive);

		float const startWidth = 0.9f * playerLabelWidthRelativeOnScreen;
		float const endWidth = 1.5f * playerLabelWidthRelativeOnScreen;
		float width = 0;
		if(isAnimationActive) {
			if(*percentSkipAnimOrNone < 0.5f) {
				width = interpolateLinear(*percentSkipAnimOrNone, 0, startWidth, 0.5f, endWidth);
			} else {
				width = interpolateLinear(*percentSkipAnimOrNone, 0.5f, endWidth, 1, startWidth);
			}

			skipAnimElement->setMaxWidth({width, egui::RelativityMode::RELATIVE_ON_SCREEN});

			float marginLeft = (playerLabelWidthRelativeOnScreen - width) / 2.0f;
			int marginLeftPx = egui::x_percentToPixel(marginLeft);
			float playerLabelHeightRelativeOnScreen = playerLabelWidthRelativeOnScreen / avatarAspectRatio * (egui::getDisplayHandler().getWidth() / float(egui::getDisplayHandler().getHeight()));
			float elemHeight = skipAnimElement->getComputedHeight();
			float marginTop = (playerLabelHeightRelativeOnScreen - elemHeight) / 2.0f;
			int marginTopPx = egui::y_percentToPixel(marginTop);

			skipAnimElement->setXTranslation(marginLeftPx);
			skipAnimElement->setYTranslation(marginTopPx + 6);
		}
	}
	void MauMauPlayerLabelOverlayRenderer::updateMauElement(std::optional<float> percentMauAnimOrNone) {
		bool isAnimationActive = percentMauAnimOrNone.has_value();
		mauAnimElement->setVisible(isAnimationActive);
		mauAnimElement->setMaxWidth({1.65f * playerLabelWidthRelativeOnScreen, egui::RelativityMode::RELATIVE_ON_SCREEN});
	}
	
}