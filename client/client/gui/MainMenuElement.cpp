#include "MainMenuElement.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodeComponents/border/Border.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <egui/model/positioning/CenterXInParentWrapper.h>
#include <array>
#include "../utils/FileUtils.h"

#include <res/menu/singlePlayer.png.h>
#include <res/menu/singlePlayerHover.png.h>
#include <res/menu/createOnlineRoom.png.h>
#include <res/menu/createOnlineRoomHover.png.h>
#include <res/menu/information.png.h>
#include <res/menu/informationHover.png.h>
#include <res/menu/joinOnlineRoom.png.h>
#include <res/menu/joinOnlineRoomHover.png.h>
#include <res/menu/createOnlineRoom.png.h>
#include <res/menu/createOnlineRoomHover.png.h>

using namespace egui;

namespace card {
	MainMenuElement::MainMenuElement() {
		float const SPACE_BETWEEN_BUTTONS = 0.1f;

		this->singlePlayerBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		singlePlayerBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_singlePlayer, tex_menu_singlePlayer_size)
		));
		singlePlayerBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_singlePlayerHover, tex_menu_singlePlayerHover_size)
		), egui::RenderMode::RENDER_EXCLUSIVELY);
		this->createOnlineRoomBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		createOnlineRoomBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_createOnlineRoom, tex_menu_createOnlineRoom_size)
		));
		createOnlineRoomBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_createOnlineRoomHover, tex_menu_createOnlineRoomHover_size)
			), egui::RenderMode::RENDER_EXCLUSIVELY);
		this->joinOnlineRoomBtn = std::make_shared<egui::AspectRatioElement>(1.0f);
		joinOnlineRoomBtn->setBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_joinOnlineRoom, tex_menu_joinOnlineRoom_size)
		));
		joinOnlineRoomBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_joinOnlineRoomHover, tex_menu_joinOnlineRoomHover_size)
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
			egui::Image::loadFromMemory(tex_menu_information, tex_menu_information_size)
		));
		infoBtn->setHoveredBackground(std::make_shared<egui::TexturedBackground>(
			egui::Image::loadFromMemory(tex_menu_informationHover, tex_menu_informationHover_size)
			), egui::RenderMode::RENDER_EXCLUSIVELY);

		
		auto gameTitleImage = egui::Image::loadTexture(getApplicationFolder() + "\\resources\\maumau.png");
		auto gameTitleImageElement = std::make_shared<egui::AspectRatioElement>(gameTitleImage->getAspectRatio());
		addChildElement(gameTitleImageElement);
		gameTitleImageElement->setMaxHeight({0.275f, egui::RelativityMode::RELATIVE_IN_PARENT});
		gameTitleImageElement->setOwnPositioning(std::make_shared<egui::CenterXInParentWrapper>(
			std::make_shared<egui::RelativePositioningOnScreen>(
				0.0f, 0.075f
			)
		));
		gameTitleImageElement->setBackground(std::make_shared<egui::TexturedBackground>(gameTitleImage));
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