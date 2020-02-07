#include "MainMenuElement.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodeComponents/border/Border.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <array>
#include "../utils/FileUtils.h"

using namespace egui;

namespace card {
	MainMenuElement::MainMenuElement() {
		float const SPACE_BETWEEN_BUTTONS = 0.1f;

		this->singlePlayerBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		singlePlayerBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\singlePlayer.png")
		));
		singlePlayerBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\singlePlayerHover.png")
		), egui::RenderMode::RENDER_EXCLUSIVELY);
		this->createOnlineRoomBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		createOnlineRoomBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\createOnlineRoom.png")
		));
		createOnlineRoomBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\createOnlineRoomHover.png")
		), egui::RenderMode::RENDER_EXCLUSIVELY);
		this->joinOnlineRoomBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		joinOnlineRoomBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\joinOnlineRoom.png")
		));
		joinOnlineRoomBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\joinOnlineRoomHover.png")
		), egui::RenderMode::RENDER_EXCLUSIVELY);

		std::shared_ptr<egui::HBox> btnBar(new egui::HBox({singlePlayerBtn, joinOnlineRoomBtn,  createOnlineRoomBtn}));
		btnBar->setSpaceBetweenElements({SPACE_BETWEEN_BUTTONS, RelativityMode::RELATIVE_ON_SCREEN});
		btnBar->setShouldCenterElements(true);
		btnBar->setOwnPositioning(std::make_shared<RelativePositioningOnScreen>(0.0f, 0.6f));
		this->addChildElement(btnBar);

		std::array<std::shared_ptr<egui::AspectRatioElement>, 3> buttons{singlePlayerBtn, joinOnlineRoomBtn, createOnlineRoomBtn};
		for(auto& btn : buttons) {
			btn->setRadius(0.1f, true);
			btn->setBorder(std::make_shared<egui::Border>(egui::Color(0.71f, 0.71f, 0.71f), egui::BorderType::SOLID, 1.0f));
			btn->setMaxDimension({0.1f, RelativityMode::RELATIVE_ON_SCREEN}, {0.3f, RelativityMode::RELATIVE_ON_SCREEN});
		}
		this->infoBtn = std::make_shared<egui::AspectRatioElement>(1.0f);

		this->addChildElement(infoBtn);
		infoBtn->setMaxHeight({0.05f, RelativityMode::RELATIVE_ON_SCREEN});
		infoBtn->setOwnPositioning(std::make_shared<egui::RelativePositioningOnScreen>(0.005f, 1 - 0.07f));
		infoBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\information.png")
		));
		infoBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadTexture(card::getApplicationFolder() + "\\resources\\menu\\informationHover.png")
		), egui::RenderMode::RENDER_EXCLUSIVELY);
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
	void MainMenuElement::addOnInfoBtnHandler(egui::FunctionWrapper<egui::ActionEvent> callback) {
		auto convertedCallback = convertEventHandler(callback);
		infoBtn->getMouseClickedEventManager().addEventHandler(convertedCallback);
	}
	egui::FunctionWrapper<egui::MouseEvent> MainMenuElement::convertEventHandler(egui::FunctionWrapper<egui::ActionEvent> constWrappedHandler) {
		return {[constWrappedHandler](egui::MouseEvent&) {
			auto wrappedHandler = constWrappedHandler;
			egui::ActionEvent evt;
			wrappedHandler(evt);
		}};
	}
}