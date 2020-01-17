#include "CardIndexRenderer.h"
#include "CardIndexRenderer.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include "../utils/FileUtils.h"
#include "../utils/DimensionsConversionUtils.h"
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>

namespace card {
	CardIndexRenderer::CardIndexRenderer(Renderer2D& renderer2D, CardIndexTextures& cardIndexTextures) :
			renderer2D(renderer2D),
			cardIndexTextures(cardIndexTextures),
			letteringTexture(
				SimpleTextureFactory(getApplicationFolder() + "\\resources\\nextCardIndex.png")
				.setMagFilter(TextureMagFilter::NEAREST)
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR)
				.generateTexture()
			){

		parentElement = std::make_shared<egui::VBox>();
		title = std::make_shared<egui::Label>();
		imageElement = std::make_shared<egui::AspectRatioElement>(cardIndexTextures.getAspectRatio());
		parentElement->addChildElement(title);		
		parentElement->addChildElement(imageElement);
		scene.setRootElement(parentElement);

		parentElement->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(
			std::make_shared<egui::RelativePositioningOnScreen>(0.0f, 0.44f)
		));
		parentElement->setPreferredWidth({0.05f, egui::RelativityMode::RELATIVE_ON_SCREEN});
		parentElement->setSpaceBetweenElements({10, egui::RelativityMode::ABSOLUTE_VALUE});
		title->setPreferredHeight({50, egui::RelativityMode::ABSOLUTE_VALUE});
		float const TITLE_STRETCH = 20;
		title->setXTranslation(-TITLE_STRETCH/2);
		title->setStretchX(TITLE_STRETCH);
		imageElement->setMaxWidth({1, egui::RelativityMode::RELATIVE_IN_PARENT});
	}
	void CardIndexRenderer::renderCardIndexForNextCard(CardIndex indexToRender) {
		if(indexToRender == CardIndex::NULLINDEX) return;

		cardIndexTextures.getTexture(indexToRender).bind();
		renderer2D.render(imageElement, true);

		letteringTexture.bind();
		renderer2D.render(title, true);
	}
}