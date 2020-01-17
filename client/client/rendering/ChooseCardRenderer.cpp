#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ChooseCardRenderer.h"
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

namespace card {
	ChooseCardRenderer::ChooseCardRenderer(egui::MasterRenderer& eguiRenderer, CardIndexTextures& cardIndexTextures, std::function<void(CardIndex)> chooseIndexFunction) :
				eguiRenderer(eguiRenderer),
				chooseIndexFunction(chooseIndexFunction) {
		
		float const BUTTON_WIDTH = 0.1f;
		float const DISTANCE_BETWEEN_BUTTONS = 0.1f;

		for(int i = 0; i < 4; i++) {
			auto btn = std::make_shared<egui::Button>();
			indexButtons.push_back(btn);

			btn->setPreferredDimension({{BUTTON_WIDTH, egui::RelativityMode::RELATIVE_ON_SCREEN}}, {{0, egui::RelativityMode::RELATIVE_ON_SCREEN}});
			btn->setBorder(std::make_shared<egui::Border>(egui::Color(), egui::BorderType::DISABLED));
		}

		indexButtons[0]->getActionEventManager().addEventHandler({[this](egui::ActionEvent&) {
			this->chooseIndexFunction(CardIndex::CLUB);
		}});
		indexButtons[0]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::CLUB)));
		indexButtons[1]->getActionEventManager().addEventHandler({[this](egui::ActionEvent&) {
			this->chooseIndexFunction(CardIndex::DIAMOND);
		}});
		indexButtons[1]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::DIAMOND)));
		indexButtons[2]->getActionEventManager().addEventHandler({[this](egui::ActionEvent&) {
			this->chooseIndexFunction(CardIndex::HEART);
		}});
		indexButtons[2]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::HEART)));
		indexButtons[3]->getActionEventManager().addEventHandler({[this](egui::ActionEvent&) {
			this->chooseIndexFunction(CardIndex::SPADE);
		}});
		indexButtons[3]->setBackground(std::make_shared<egui::TexturedBackground>(cardIndexTextures.getImage(CardIndex::SPADE)));

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
		buttonBarBackground->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1, 1, 1, 0.85f)));

		std::shared_ptr<egui::UnorganizedParentElement> rootElem(new egui::UnorganizedParentElement({buttonBarBackground, buttonBar}));
		rootElem->setBackground(std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, 0.55f)));
		this->scene.setRootElement(rootElem);
	}
	void ChooseCardRenderer::resetScene() {
		scene.discardMouseEvents();
	}
	void ChooseCardRenderer::render() {
		adjustButtonBar();

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void card::ChooseCardRenderer::adjustButtonBar() {
		int btnWidthInPixels = egui::x_percentToPixel(indexButtons[0]->getComputedWidth());
		float aspiredButtonHeightInPercent = egui::y_pixelToPercent(btnWidthInPixels);
		egui::ValuePart preferredHeight(aspiredButtonHeightInPercent, egui::RelativityMode::RELATIVE_ON_SCREEN);
		for(auto& btn : indexButtons) {
			btn->setPreferredHeight({preferredHeight});
		}

		buttonBar->setPreferredHeight({preferredHeight});
		buttonBarBackground->setPreferredHeight({{preferredHeight, {0.05f, egui::RelativityMode::RELATIVE_ON_SCREEN}}});
	}
}