#include "MauMauPlayerLabelOverlayRenderer.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include "../utils/FileUtils.h"
#include "../gui/CombinedPositioning.h"
#include <res/ingame/mauspeechbubble.h>
#include <res/ingame/skipPlayer.png.h>

namespace card {
	MauMauPlayerLabelOverlayRenderer::MauMauPlayerLabelOverlayRenderer(Renderer2D& renderer2D, float playerLabelWidthRelativeOnScreen) :
			renderer2D(renderer2D),
			playerLabelWidthRelativeOnScreen(playerLabelWidthRelativeOnScreen),
			textureSkip(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromMemory(tex_skipPlayer, tex_skipPlayer_size)),
			textureSkipGrey(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromFile("C:\\Users\\Elias\\Documents\\PROJECT X\\skipgrey.png")),
			textureMau(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromMemory(tex_mauspeechbubble, tex_mauspeechbubble_size)),
			skipAnimElement(std::make_shared<egui::AspectRatioElement>(textureSkip.getAspectRatio())),
			skipAnimGreyElement(std::make_shared<egui::AspectRatioElement>(textureSkipGrey.getAspectRatio())),
			mauAnimElement(std::make_shared<egui::AspectRatioElement>(textureMau.getAspectRatio())),
			basicPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.0f)) {
			
		skipAnimElement->setOwnPositioning(basicPositioning);
		skipAnimGreyElement->setOwnPositioning(basicPositioning);

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

		updateSkipElement(percentSkipAnimOrNone);
		updateSkipGreyElement(renderSkipGrey);
		updateMauElement(percentMauAnimOrNone);

		if(percentSkipAnimOrNone.has_value()) {
			textureSkip.bind();
			renderer2D.render(skipAnimElement, true);
		}
		if(renderSkipGrey) {
			textureSkipGrey.bind();
			renderer2D.render(skipAnimGreyElement, true);
		}
		if(percentMauAnimOrNone.has_value()) {
			textureMau.bind();
			renderer2D.render(mauAnimElement, true);
		}
	}
	void MauMauPlayerLabelOverlayRenderer::updateSkipElement(std::optional<float> percentSkipAnimOrNone) {
		bool isAnimationActive = percentSkipAnimOrNone.has_value();
		skipAnimElement->setVisible(isAnimationActive);

		int computedXTranslation = 25;
		int computedYTranslation = 5;
		if(isAnimationActive) {
			// interpolation
			if(*percentSkipAnimOrNone > 0.5f) percentSkipAnimOrNone = (*percentSkipAnimOrNone - 1) * (-2);
			else percentSkipAnimOrNone = (*percentSkipAnimOrNone) * 2;

			computedXTranslation += 7 * (*percentSkipAnimOrNone);
			computedYTranslation += 7 * (*percentSkipAnimOrNone);
		}

		skipAnimElement->setMaxWidth({{playerLabelWidthRelativeOnScreen, egui::RelativityMode::RELATIVE_ON_SCREEN}, {2.0f * computedXTranslation, egui::RelativityMode::ABSOLUTE_VALUE}});
		skipAnimElement->setXTranslation(-computedXTranslation);
		skipAnimElement->setYTranslation(-computedYTranslation);
	}
	void MauMauPlayerLabelOverlayRenderer::updateSkipGreyElement(bool shouldRender) {
		skipAnimGreyElement->setVisible(shouldRender);

		float skipGreyElementWidth = 0.9f * playerLabelWidthRelativeOnScreen;
		float marginLeft = (playerLabelWidthRelativeOnScreen - skipGreyElementWidth) / 2.0f;
		int marginLeftPx = egui::x_percentToPixel(marginLeft);
		
		skipAnimGreyElement->setMaxWidth({{skipGreyElementWidth, egui::RelativityMode::RELATIVE_ON_SCREEN}});
		skipAnimGreyElement->setXTranslation(marginLeftPx);
		skipAnimGreyElement->setYTranslation(12);
	}
	void MauMauPlayerLabelOverlayRenderer::updateMauElement(std::optional<float> percentMauAnimOrNone) {
		bool isAnimationActive = percentMauAnimOrNone.has_value();
		mauAnimElement->setVisible(isAnimationActive);
		mauAnimElement->setMaxWidth({1.65f * playerLabelWidthRelativeOnScreen, egui::RelativityMode::RELATIVE_ON_SCREEN});
	}
	
}