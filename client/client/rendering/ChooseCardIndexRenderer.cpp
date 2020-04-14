#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ChooseCardIndexRenderer.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <egui/model/positioning/CenterAllInParentWrapper.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/utils/PositioningUtils.h>
#include <egui/model/nodeComponents/background/DisabledBackground.h>

#include <iostream>
#include "../utils/FileUtils.h"
#include <res/cardIndex/CardIndexHover.png.h>
namespace card {
	ChooseCardIndexRenderer::ChooseCardIndexRenderer(egui::MasterRenderer& eguiRenderer, CardIndexTextures& cardIndexTextures) :
				eguiRenderer(eguiRenderer),
				messageRenderer(eguiRenderer) {
		
		MessageKey msgKey;
		messageQueue.appendMessagePermanently(u8"Wähle die zu spielende Kartenfarbe aus.", msgKey);
		
		float const BUTTON_WIDTH = 0.1f;
		float const DISTANCE_BETWEEN_BUTTONS = 0.1f;

		for(int i = 0; i < 4; i++) {
			auto btn = std::make_shared<egui::Button>();
			indexButtons.push_back(btn);

			btn->setPreferredDimension({{BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{0, egui::RelativityMode::RELATIVE_ON_SCREEN}});
			btn->setBorder(std::make_shared<egui::Border>(egui::Color(), egui::BorderType::DISABLED));
		}

		indexButtons[0]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::HEART)));
		indexButtons[0]->setHoveredBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_cardIndex_heartHover, tex_cardIndex_heartHover_size)), egui::RenderMode::RENDER_EXCLUSIVELY);
		
		indexButtons[1]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::SPADE)));
		indexButtons[1]->setHoveredBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_cardIndex_spadeHover, tex_cardIndex_spadeHover_size)), egui::RenderMode::RENDER_EXCLUSIVELY);

		indexButtons[2]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::DIAMOND)));
		indexButtons[2]->setHoveredBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_cardIndex_diamondHover, tex_cardIndex_diamondHover_size)), egui::RenderMode::RENDER_EXCLUSIVELY);

		indexButtons[3]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::CLUB)));
		indexButtons[3]->setHoveredBackground(std::make_shared<egui::TexturedBackground>(egui::Image::loadFromMemory(tex_cardIndex_clubHover, tex_cardIndex_clubHover_size)), egui::RenderMode::RENDER_EXCLUSIVELY);
	
		

		egui::Value buttonBarWidth(indexButtons.size() * BUTTON_WIDTH + (indexButtons.size() - 1) * DISTANCE_BETWEEN_BUTTONS, egui::RelativityMode::RELATIVE_ON_SCREEN);
		egui::Value buttonBarHeight(0, egui::RelativityMode::RELATIVE_ON_SCREEN);
		this->buttonBar = std::make_shared<egui::HBox>();
		for(auto& a : indexButtons) {
			buttonBar->addChildElement(a);
		}
		buttonBar->setOwnPositioning(std::make_shared<egui::CenterAllInParentWrapper>());
		buttonBar->setPreferredDimension(buttonBarWidth, buttonBarHeight);
		buttonBar->setSpaceBetweenElements({{DISTANCE_BETWEEN_BUTTONS, egui::RelativityMode::RELATIVE_ON_SCREEN}});

		this->buttonBarBackground = std::make_shared<egui::Label>();
		buttonBarBackground->setOwnPositioning(std::make_shared<egui::CenterAllInParentWrapper>());
		buttonBarBackground->setPreferredDimension({1, egui::RelativityMode::RELATIVE_ON_SCREEN}, {0, egui::RelativityMode::RELATIVE_ON_SCREEN});
		buttonBarBackground->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0.9f, 0.9f, 0.9f, 0.925f)));

		std::shared_ptr<egui::UnorganizedParentElement> rootElem(new egui::UnorganizedParentElement({buttonBarBackground, buttonBar}));
		rootElem->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.55f)));
		this->scene.setRootElement(rootElem);
	}
	void ChooseCardIndexRenderer::discardPreviousMouseEvents() {
		scene.discardMouseEvents();
	}
	void ChooseCardIndexRenderer::render() {
		adjustButtonBar();

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		messageRenderer.render(messageQueue);
	}
	void ChooseCardIndexRenderer::adjustButtonBar() {
		int btnWidthInPixels = egui::x_percentToPixel(indexButtons[0]->getComputedWidth());
		float aspiredButtonHeightInPercent = egui::y_pixelToPercent(btnWidthInPixels);
		egui::Value preferredHeight(aspiredButtonHeightInPercent, egui::RelativityMode::RELATIVE_ON_SCREEN);
		for(auto& btn : indexButtons) {
			btn->setPreferredHeight({preferredHeight});
		}

		buttonBar->setPreferredHeight({preferredHeight});
		buttonBarBackground->setPreferredHeight({{preferredHeight, {0.05f, egui::RelativityMode::RELATIVE_ON_SCREEN}}});
	}
	void ChooseCardIndexRenderer::addChooseIndexHandler(std::function<void(CardIndex)> chooseIndexFunction) {
		indexButtons[0]->getActionEventManager().addEventHandler({[chooseIndexFunction](egui::ActionEvent&) {
			chooseIndexFunction(CardIndex::HEART);
		}});
		indexButtons[1]->getActionEventManager().addEventHandler({[chooseIndexFunction](egui::ActionEvent&) {
			chooseIndexFunction(CardIndex::SPADE);
		}});
		indexButtons[2]->getActionEventManager().addEventHandler({[chooseIndexFunction](egui::ActionEvent&) {
			chooseIndexFunction(CardIndex::DIAMOND);
		}});
		indexButtons[3]->getActionEventManager().addEventHandler({[chooseIndexFunction](egui::ActionEvent&) {
			chooseIndexFunction(CardIndex::CLUB);
		}});
	}
}