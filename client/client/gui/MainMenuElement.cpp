#include "MainMenuElement.h"
#include "MainMenuElement.h"
#include "MainMenuElement.h"
#include "MainMenuElement.h"
#include "MainMenuElement.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <array>
using namespace egui;

namespace card {
	MainMenuElement::MainMenuElement() {
		float const SPACE_BETWEEN_BUTTONS = 0.1f;

		this->singlePlayerBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		this->createOnlineRoomBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		this->joinOnlineRoomBtn = std::make_shared<egui::AspectRatioElement>(1.0f);

		std::shared_ptr<egui::HBox> btnBar(new egui::HBox({singlePlayerBtn, createOnlineRoomBtn, joinOnlineRoomBtn}));
		btnBar->setSpaceBetweenElements({SPACE_BETWEEN_BUTTONS, RelativityMode::RELATIVE_ON_SCREEN});
		btnBar->setShouldCenterElements(true);
		btnBar->setOwnPositioning(std::make_shared<RelativePositioningOnScreen>(0.0f, 0.6f));
		this->addChildElement(btnBar);

		std::array<std::shared_ptr<egui::AspectRatioElement>, 3> buttons{singlePlayerBtn, createOnlineRoomBtn, joinOnlineRoomBtn};
		for(auto& btn : buttons) {
			btn->setBackground(std::make_shared<ColoredBackground>(Color(1.0f, 0.15f, 0.2f)));
			btn->setMaxDimension({0.1f, RelativityMode::RELATIVE_ON_SCREEN}, {0.3f, RelativityMode::RELATIVE_ON_SCREEN});
		}
	}
	void MainMenuElement::addOnSinglePlayerBtnHandler(egui::FunctionWrapper<egui::ActionEvent> callback) {
		auto convertedCallback = convertEventHandler(callback);
		singlePlayerBtn->getMouseClickedEventManager().addEventHandler(convertedCallback);
	}
	void MainMenuElement::addOnCreateOnlineRoomHandler(egui::FunctionWrapper<egui::ActionEvent> callback) {
		auto convertedCallback = convertEventHandler(callback);
		createOnlineRoomBtn->getMouseClickedEventManager().addEventHandler(convertedCallback);
	}
	void MainMenuElement::addOnJoinOnlineRoomHandler(egui::FunctionWrapper<egui::ActionEvent> callback) {
		auto convertedCallback = convertEventHandler(callback);
		joinOnlineRoomBtn->getMouseClickedEventManager().addEventHandler(convertedCallback);

	}
	egui::FunctionWrapper<egui::MouseEvent> MainMenuElement::convertEventHandler(egui::FunctionWrapper<egui::ActionEvent> constWrappedHandler) {
		return {[constWrappedHandler](egui::MouseEvent&) {
			auto wrappedHandler = constWrappedHandler;
			egui::ActionEvent evt;
			wrappedHandler(evt);
		}};
	}
}